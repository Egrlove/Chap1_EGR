#include "Lesson101.h"
#include "Timing.h"
#include <vector>
#include <iterator> 
#include <time.h>
#include <cstdlib>

struct sphere_param
{
	NxVec3 color;
	int last_point;
	int lastPoint;
	vector<NxVec3> trace;
	vector<NxVec3> way;
	vector<NxVec3>::iterator its;
	vector<NxVec3>::iterator next_point_for_deleting;
	NxActor* selc_Ac;
};




extern  vector<sphere_param> objects;
extern vector<NxVec3> vectr_shortest_way;
extern NxVec3** vec_matrix;
extern int** adj_matrix;


extern int SpheresNumber  ;
extern int X_bro  ;
extern int Y_bro  ;
extern int STEP  ;
extern int dest  ;
extern int SCALE  ;
extern float offsetX  ;
extern float offsetY  ;
extern float offsetZ;






void
drawing_arrows(vector<NxVec3>::iterator iters,struct sphere_param &it)
{
	if ((abs((*next(iters)).x - (*iters).x) < (STEP - STEP*offsetX)) && (abs((*next(iters)).z - (*iters).z)) > STEP / 2)
	{
		if ((*next(iters)).z - (*iters).z > 0) {
			NxVec3 center_of_line = (*next(iters) + *iters) / 2;
			DrawTriangle(center_of_line, center_of_line + NxVec3(1, 0, -1), center_of_line + NxVec3(-1, 0, -1), (&it)->color);
		}
		else {
			NxVec3 center_of_line = (*iters + *next(iters)) / 2;
			DrawTriangle(center_of_line, center_of_line + NxVec3(-1, 0, 1), center_of_line + NxVec3(1, 0, 1), (&it)->color);
		}
	}

	if ((abs((*next(iters)).z - (*iters).z) < (STEP - STEP*offsetZ)) && (abs((*next(iters)).x - (*iters).x)) > STEP / 2)
	{
		if ((*next(iters)).x - (*iters).x > 0) {
			NxVec3 center_of_line = (*next(iters) + *iters) / 2;
			DrawTriangle(center_of_line, center_of_line + NxVec3(-1, 0, -1), center_of_line + NxVec3(-1, 0, 1), (&it)->color);
		}
		else {
			NxVec3 center_of_line = (*iters + *next(iters)) / 2;
			DrawTriangle(center_of_line, center_of_line + NxVec3(1, 0, 1), center_of_line + NxVec3(1, 0, -1), (&it)->color);
		}
	}

}




void drawing_graph()
{
	int pos_y_s;
	int pos_x_s;
	int pos_y_e;
	int pos_x_e;

	for (int i = 0; i < X_bro * Y_bro; i++)
		for (int j = 0; j < X_bro * Y_bro; j++)
		{
			if (adj_matrix[i][j])
			{
				pos_y_s = i / X_bro;
				pos_x_s = i - pos_y_s*X_bro;
				pos_y_e = j / X_bro;
				pos_x_e = j - pos_y_e *X_bro;
				//cout << "POSITIONS   " << pos_x_s << " " << pos_y_s << " " << pos_x_e << " " << pos_y_e<<endl;
				DrawLine(vec_matrix[pos_x_s][pos_y_s], vec_matrix[pos_x_e][pos_y_e], NxVec3(1, 1, 1), 1.0);
			}
		}
}