
#include <stdlib.h>
#include <time.h>

#include <reslib.h>

TYPEDEF_STRUCT (box_t,
                (char *, name),
                (time_t, creation_time)
                )

TYPEDEF_STRUCT (voxel_t,
                (box_t *, pbox)
                )

TYPEDEF_STRUCT (plane_t,
                (voxel_t, points, [2][2]),
                )

TYPEDEF_STRUCT (space_t,
                (plane_t, planes, [2])
                )

int main ()
{
  space_t space;
  box_t box = { .name = "some name", .creation_time = time (NULL) };

  memset (&space, 0, sizeof(space));
  
  space.planes[0].points[0][0].pbox = &box;
  
  // here goes the rest of the code
  char * space_serialized = MR_SAVE_CINIT (space_t, &space);
  if (NULL == space_serialized)
    printf ("Serialization failed\n");
  else
    {
      printf ("space_t space = %s;\n", space_serialized);
      MR_FREE (space_serialized);
    }
  return (EXIT_SUCCESS);
}
