/* -*- C -*- */
/* This file is a part of Metaresc project */

/*
  Replays inputs through LLVMFuzzerTestOneInput without libFuzzer.

  Usage: fuzz_xdr_load_replay [file or directory]...
  Without arguments it replays the corpus directory that was compiled in
  with -DFUZZ_CORPUS_DIR, which is how "make check" runs it. A crash or a
  sanitizer report makes the program fail, so every input in the corpus
  is a regression test.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

extern int LLVMFuzzerInitialize (int * argc, char *** argv);
extern int LLVMFuzzerTestOneInput (const uint8_t * data, size_t size);

static int inputs_count = 0;

static int
replay_file (const char * path)
{
  FILE * fp = fopen (path, "rb");
  if (NULL == fp)
    {
      fprintf (stderr, "cannot open %s\n", path);
      return (1);
    }

  long size = (fseek (fp, 0, SEEK_END) == 0) ? ftell (fp) : -1;
  if ((size < 0) || (fseek (fp, 0, SEEK_SET) != 0))
    {
      fprintf (stderr, "cannot get size of %s\n", path);
      fclose (fp);
      return (1);
    }

  uint8_t * data = malloc (size ? size : 1);
  if ((NULL == data) || (fread (data, 1, size, fp) != (size_t)size))
    {
      fprintf (stderr, "cannot read %s\n", path);
      free (data);
      fclose (fp);
      return (1);
    }
  fclose (fp);

  LLVMFuzzerTestOneInput (data, size);
  free (data);
  ++inputs_count;
  return (0);
}

static int
replay_path (const char * path)
{
  struct stat st;
  if (stat (path, &st) != 0)
    {
      fprintf (stderr, "cannot access %s\n", path);
      return (1);
    }

  if (!S_ISDIR (st.st_mode))
    return (replay_file (path));

  DIR * dir = opendir (path);
  if (NULL == dir)
    {
      fprintf (stderr, "cannot open directory %s\n", path);
      return (1);
    }

  int failed = 0;
  struct dirent * entry;
  while ((entry = readdir (dir)) != NULL)
    {
      if (entry->d_name[0] == '.')
        continue;
      size_t length = strlen (path) + strlen (entry->d_name) + 2;
      char * file = malloc (length);
      if (NULL == file)
        {
          failed = 1;
          break;
        }
      snprintf (file, length, "%s/%s", path, entry->d_name);
      failed |= replay_path (file);
      free (file);
    }
  closedir (dir);
  return (failed);
}

int
main (int argc, char * argv[])
{
  int failed = 0;
  int i;

  LLVMFuzzerInitialize (&argc, &argv);

  if (argc > 1)
    for (i = 1; i < argc; ++i)
      failed |= replay_path (argv[i]);
  else
#ifdef FUZZ_CORPUS_DIR
    failed = replay_path (FUZZ_CORPUS_DIR);
#else
    {
      fprintf (stderr, "usage: %s file-or-directory...\n", argv[0]);
      return (2);
    }
#endif

  printf ("replayed %d inputs\n", inputs_count);
  /* an empty corpus would turn the test into a no-op */
  return ((failed || (0 == inputs_count)) ? EXIT_FAILURE : EXIT_SUCCESS);
}
