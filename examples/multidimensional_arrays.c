
#include <stdlib.h>
#include <time.h>

#include <metaresc.h>

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
                (plane_t, planes, [2]),
                )

int main ()
{
  space_t space;
  box_t box = { .name = "some name", .creation_time = time (NULL) };

  memset (&space, 0, sizeof(space));

  space.planes[0].points[0][0].pbox = &box;

  // here we will serialize and print to console
  MR_PRINT ("space_t space = ", (space_t, &space));

  return (EXIT_SUCCESS);
}
