
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
  
  space.planes[3].points[2][5].pbox = RL_MALLOC(sizeof(box_t));
  space.planes[3].points[2][5].pbox->creation_time = time(NULL);
  
  // here goes the rest of the code
  
  return (EXIT_SUCCESS);
}
