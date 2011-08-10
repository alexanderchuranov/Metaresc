
#include <stdlib.h>
#include <time.h>

#include <reslib.h>

TYPEDEF_STRUCT (box_t,
                (char*, name),
                (time_t, creation_time)
                )

TYPEDEF_STRUCT (voxel_t,
                (box_t*, pbox)
                )

TYPEDEF_STRUCT (plane_t,
                (voxel_t, points, [10][10]),
                )

TYPEDEF_STRUCT (space_t,
                (plane_t, planes, [10])
                )

int main()
{
  space_t space;
  box_t box = { .name = "some name", .creation_time = time (NULL) };

  memset(&space, 0, sizeof(space));
  
  space.planes[3].points[2][5].pbox = &box;
  
  // here goes the rest of the code
  puts(RL_SAVE_JSON(space_t, &space));
  
  return (EXIT_SUCCESS);
}
