#include "Lesson101.h"
#include "Timing.h"
#include <vector>
#include <iterator> 
#include <time.h>
#include <cstdlib>
#include "tinyxml.h"
#include "tinystr.h"
#define TIXML_USE_STL

void
drawing_arrows(vector<NxVec3>::iterator ,struct sphere_param & );
void
drawing_graph();
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

static vector<sphere_param> objects;
vector<NxVec3> vectr_shortest_way;
NxVec3** vec_matrix;
int** adj_matrix;

int SpheresNumber = 3;
int X_bro = 4;
int Y_bro = 4;
int STEP = 5;
int dest = 0;
int SCALE = 1000;
float offsetX = 0.25;
float offsetY = 0.25;
float offsetZ = 0.25;




static int go = 0;
int counts;


NxVec3 randomcolor()
{
	float R, G, B;

	R = float((rand() % 1000 )) / 1000;
	G = float((rand() % 1000 )) / 1000;
	B = float((rand() % 1000 )) / 1000;
	return NxVec3(R,G,B);

}

void print_NXvectr(NxVec3 null_vectr)
{
	cout << null_vectr[0] << " " << null_vectr[1] << " " << null_vectr[2] << " | ";
}

vector<NxVec3> makingvec(vector<NxVec3> v)
{
	int i = 1;
	NxVec3 first;
	NxVec3 second;
	NxVec3 vectr_direction;
	NxVec3 delta;
	NxVec3 null_vectr;
	vector<NxVec3>  vectr;

	for (auto it = v.begin(); it != prev(v.end()); ++it)
	{
		if (it == v.begin()) {
			vectr.emplace_back(*it);
		}

		auto next_it = next(it);
		first = *it;
		second = *next_it;
		vectr_direction = second - first;

		NxReal elapTime = getElapsedTime();
		

		float deltaTime = 0.0001;
		deltaTime = 2;// /= elapTime;
		int count = 100 / deltaTime;
		delta = vectr_direction / SCALE;
		//printf("getET = %f\n", elapTime);
		//prev = elapTime;

		for (auto vectr_iter = vectr.end(); i < SCALE ; ++i)
		{
			vectr.emplace_back((*prev(vectr.end())) + delta);
			//print_NXvectr(*prev(vectr.end()) );
		}
		i = 0;
	}
	return vectr;
}

void making_matrix()
{

	vec_matrix = new NxVec3*[Y_bro];
	for (int i = 0; i < Y_bro; ++i)
	{
		vec_matrix[i] = new NxVec3[X_bro];
	}
	
	srand(time(NULL));

	for (int i = 0; i < Y_bro; i++)
		for (int j = 0; j < X_bro; j++){			
			float X_s =  (float)j + float(rand() % int(offsetX * 100))/ 100;
			float Y_s = float(rand() % int(offsetY * 100)) / 100;
			float Z_s = (float)i + float(rand() % int(offsetZ * 100)) / 100;
			vec_matrix[j][i] = NxVec3(X_s, Y_s , Z_s) *STEP;
			//cout << X_s << " " << Y_s << " " << Z_s << " | ";
		}
	//for (int i = 0; i < Y_bro; i++)
	//{
	//	for (int j = 0; j < X_bro; j++)
	//	{
	//		print_NXvectr(vec_matrix[i][j]);
	//	}
	//	cout << endl;
	//}
}

void generatig_graph()
{
	int f, num;

	//cin >> X_bro;
	//cin >> Y_bro;


	adj_matrix = new int*[X_bro*Y_bro];

	for (int i = 0; i < X_bro*Y_bro; ++i)
	{
		adj_matrix[i] = new int[X_bro*Y_bro];
	}

	for (int i = 0; i < X_bro * Y_bro; i++)
		for (int j = 0; j < X_bro * Y_bro; j++)
			adj_matrix[i][j] = 0;

	for (int i = 0; i < Y_bro - 1; i++)
		for (int j = 0; j < X_bro; j++)
		{
			int pos = i * X_bro + j;
			int sur_270 = (i + 1) * X_bro + j;

			adj_matrix[pos][sur_270] = 1;
			adj_matrix[sur_270][pos] = 1;
		}

	int i = 0;

	for (int j = 0; j < X_bro - 1; j++)
	{
		int pos = i * X_bro + j;
		int sur_0 = i * X_bro + j + 1;

		adj_matrix[pos][sur_0] = 1;
		adj_matrix[sur_0][pos] = 1;
	}

	num = (X_bro*Y_bro - X_bro) / 2;
	srand(time(NULL));
	int step = 1 + Y_bro / 50;


	for (int i = 0; num && i < Y_bro; i += step)
		for (int j = 0; num && j < X_bro - 1; j++)
		{
			f = rand() % 3;
			if (f == 2){
				int pos = i * X_bro + j;
				int sur_0 = i * X_bro + j + 1;

				adj_matrix[pos][sur_0] = 1;
				adj_matrix[sur_0][pos] = 1;
				num--;
			}
		}

	/*for (int i = 0; i < X_bro * Y_bro; i++)
	{
		for (int j = 0; j < X_bro * Y_bro; j++)
			if (adj_matrix[i][j])
				cout << adj_matrix[i][j] << " ";
			else
				cout << "_" << " ";
		cout << endl;
	} */
}

int* deikstr(int A, int B)
{
	struct dei
	{
		int name;
		int num;
	};

	struct dei **dei_matrix = NULL;
	int *pathway = NULL;
	int f = 0;
	int k = -1;
	int num_of_steps = 1;
	int n = 0;

	dei_matrix = new struct dei*[2*X_bro*Y_bro];


	struct dei parent;
	parent.name = A;
	parent.num = 1;
	int flag = 0;

	while (!flag)
	{
		k++;
		dei_matrix[k] = new struct dei[X_bro*Y_bro];
		for (int i = 0; i < X_bro * Y_bro; i++)
		{
			if (!k) {
				if (adj_matrix[parent.name][i])
				{
					dei_matrix[k][i].name = parent.name;
					dei_matrix[k][i].num = parent.num;
				}
				else
					dei_matrix[k][i].name = -1;
			}
			else
			{
				if (adj_matrix[parent.name][i] && (dei_matrix[k - 1][i].name < 0 || dei_matrix[k - 1][i].num > parent.num))
				{
					dei_matrix[k][i].name = parent.name;
					dei_matrix[k][i].num = parent.num + 1;
				}
				else
				{
					//dei_matrix[k][i] = dei_matrix[k - 1][i];
					dei_matrix[k][i].name = dei_matrix[k - 1][i].name;
					dei_matrix[k][i].num = dei_matrix[k - 1][i].num;
				}
			}
		}

		f = 0;
		int b = 0;

		for (int i = 0; i < X_bro * Y_bro; ++i)
		{
			if (dei_matrix[k][i].num == num_of_steps)
			{
				if (b == n)
				{
					f = 1;
					parent.name = i;
					parent.num = dei_matrix[k][i].num;
				}
				b++;
			}
		}

		if (f == 0)
		{
			num_of_steps++;
			n = 0;
		}
		else
		{
			n++;
		}

		if (dei_matrix[k][B].name != -1)
		{
			flag = 1;
		}
	}

	int name = B;
	int numb = dei_matrix[k][B].num;
	counts = numb + 1; // кол-во

	pathway = new int[counts];
	pathway[numb] = B;
	numb--;

	while (numb > -1)
	{
		name = dei_matrix[k][name].name;
		pathway[numb] = name;
		numb--;
	}


	for (int i = 0; i < k; i++)
	{
		delete dei_matrix[i];
	}

	delete[] dei_matrix;

	return pathway;
}


vector<NxVec3> making_vec_from_dijkstra(int A)
{
	dest = rand() % (X_bro * Y_bro);

	int *pathway = deikstr(A, dest);
	int pos_y_s;
	int pos_x_s;

	vectr_shortest_way.clear();
	for (int I = 0; I < counts; ++I) {
		pos_y_s = pathway[I] / X_bro;
		pos_x_s = pathway[I] - pos_y_s*X_bro;
		vectr_shortest_way.push_back(vec_matrix[pos_x_s][pos_y_s]);
	}
	return vectr_shortest_way;
}

void initite() {
	generatig_graph();
	making_matrix();///*
	sphere_param gg;
	if (go == 0)
	{
		for (int i = 0; i < SpheresNumber; ++i)
		{
			int A = rand() % (X_bro * Y_bro);
			gg.color = randomcolor();
			gg.way = making_vec_from_dijkstra(A);
			gg.trace = makingvec(gg.way);
			gg.last_point = dest;
			gg.its = gg.trace.begin();
			gg.selc_Ac = CreateSphere();
			gg.next_point_for_deleting = next(gg.way.begin());

			objects.push_back( move(gg));
		}
		go = 1;
	}//*/

}

int param, er;
double values[8];

void parser()
{
	double value;
	int i = 0;
	TiXmlDocument *xml_file = new TiXmlDocument("options.xml");
	if (!xml_file->LoadFile()) {
		er = 666;
		return;
	}
	else
	{

		TiXmlElement* element = xml_file->FirstChildElement("Parent");

		element = element->FirstChildElement("Child");

		while (element != NULL)
		{
			if (element->QueryIntAttribute("param", &param) == TIXML_SUCCESS
				&&
				element->QueryDoubleAttribute("value", &value) == TIXML_SUCCESS)
			{
				cout << value << endl;
				values[i] = value;
				++i;
			}
			element = element->NextSiblingElement();
		}
	}
	 SpheresNumber =values[0] ;
	 X_bro = values[1];
	 Y_bro = values[2];
	 STEP = values[3];
	 SCALE = values[4];
	 offsetX = values[5];
	 offsetY = values[6];
	 offsetZ = values[7];
}
// Physics SDK globals
NxPhysicsSDK*     gPhysicsSDK = NULL;
NxScene*          gScene = NULL;
NxVec3            gDefaultGravity(0,-9.8,0);

// User report globals
DebugRenderer     gDebugRenderer;
HUD hud;
 
// Display globals
int gMainHandle;
int mx = 0;
int my = 0;

// Camera globals
float	gCameraAspectRatio = 1.0f;
NxVec3	gCameraPos(140,50,-15);
NxVec3	gCameraForward(0,0,1);
NxVec3	gCameraRight(-1,0,0);
const NxReal gCameraSpeed = 5;

// Force globals
NxVec3	gForceVec(0,0,0);
NxReal	gForceStrength	= 20000;
bool	bForceMode		= true;

// Keyboard globals
#define MAX_KEYS 256
bool gKeys[MAX_KEYS];

// Simulation globals
NxReal	gDeltaTime			= 1.0/60.0;
bool	bHardwareScene		= false;
bool	bPause				= false;
bool	bShadows			= true;
bool	bDebugWireframeMode = false;
NxActor* groundPlane = NULL;
NxActor* gSelectedActor = NULL;


void RenderActors(bool shadows)
{
	
	vector<NxVec3>::iterator iters;
	vector<NxVec3>::iterator next_point;
	NxVec3 cur_point;

	if (go == 1){
		for (auto& it : objects){
			cur_point = *((&it)->its);
			(&it)->way[0] = cur_point;


			for (iters = (&it)->way.begin(); iters != (&it)->way.end(); ++iters ) {
				if (next(iters) != (&it)->way.end()) {
					drawing_arrows(iters, it);
					DrawLine(*iters, *next(iters), ((&it)->color), 2.0);					
				}
			}

			// для того чтобы путь начинался в шаре
			float delta = 0.6;
			//float delta = 1/(float)STEP ;
			if (((abs((*((&it)->its)).x - (*((&it)->next_point_for_deleting)).x)) < delta) && ((abs((*((&it)->its)).y - (*((&it)->next_point_for_deleting)).y)) <delta)
				&& ((abs((*((&it)->its)).z - (*((&it)->next_point_for_deleting)).z)) < delta))
			{
				if ( ((&it)->way.size()) > 2 ) {
					(&it)->way.erase(((&it)->next_point_for_deleting));
					iters = (&it)->way.begin();
					(&it)->next_point_for_deleting = next(iters);
					(&it)->way[0] = *((&it)->its);
				}
			}


			if ( ((&it)->its) != --(&it)->trace.end() ){
				cur_point = *((&it)->its);
				(&it)->selc_Ac->moveGlobalPosition(*((&it)->its++));
			}
			else{
				(&it)->trace.clear();
				(&it)->way.clear();
				(&it)->way = making_vec_from_dijkstra((&it)->last_point);
				(&it)->last_point = dest;
				(&it)->trace = makingvec((&it)->way);
				(&it)->next_point_for_deleting = next((&it)->way.begin());
				(&it)->its = (&it)->trace.begin();
			}

		}
	}


	drawing_graph();

	// Render all the actors in the scene
	NxU32 nbActors = gScene->getNbActors();
	NxActor** actors = gScene->getActors();

	while (nbActors--)
	{
		NxActor* actor = *actors++;
		DrawActor(actor, gSelectedActor, false);


		// Handle shadows
		if (shadows)
		{
			DrawActorShadow(actor, false);
		}
	}

}



//ideal
//DrawTriangle( NxVec3(0, 2, 0) ,NxVec3(1,2,-10), NxVec3(-1, 2, -10), ((&it)->color));
//Na menya
//DrawTriangle(NxVec3(0, 2, 0), NxVec3(-1, 2, 10), NxVec3(1, 2, 10), ((&it)->color));
//---


void ProcessCameraKeys()
{
	NxReal deltaTime;

    if (bPause) 
	{
		deltaTime = 0.0005;
	}
	else 
	{
		deltaTime = gDeltaTime;
	}
		
	// Process camera keys
	for (int i = 0; i < MAX_KEYS; i++)
	{	
		if (!gKeys[i])  { continue; }

		switch (i)
		{
			// Camera controls
			case 'w':{ gCameraPos += gCameraForward*gCameraSpeed*deltaTime; break; }
			case 's':{ gCameraPos -= gCameraForward*gCameraSpeed*deltaTime; break; }
			case 'a':{ gCameraPos -= gCameraRight*gCameraSpeed*deltaTime;	break; }
			case 'd':{ gCameraPos += gCameraRight*gCameraSpeed*deltaTime;	break; }
			case 'z':{ gCameraPos -= NxVec3(0,1,0)*gCameraSpeed*deltaTime;	break; }
			case 'q':{ gCameraPos += NxVec3(0,1,0)*gCameraSpeed*deltaTime;	break; }
		}
	}
}

void SetupCamera()
{
	gCameraAspectRatio = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT);
	// Setup camera
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, gCameraAspectRatio, 1.0f, 10000.0f);
	gluLookAt(gCameraPos.x,gCameraPos.y,gCameraPos.z,gCameraPos.x + gCameraForward.x, gCameraPos.y + gCameraForward.y, gCameraPos.z + gCameraForward.z, 0.0f, 1.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ProcessForceKeys()
{
	// Process force keys
	for (int i = 0; i < MAX_KEYS; i++)
	{	
		
		if (!gKeys[i])  { continue; }
		switch (i){
			case 'f': {		break; }
			case 'i': { 	break; }
			case 'k': { 	break; }
			case 'j': {		break; }
			case 'l': { 	break; }
			case 'u': {		break; }
			case 'm': { 	break; }
		}
	}
}

void ProcessInputs()
{
    ProcessForceKeys();

    // Show debug wireframes
	if (bDebugWireframeMode)
	{
		if (gScene)  gDebugRenderer.renderData(*gScene->getDebugRenderable());
	}
}

void RenderCallback()
{
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ProcessCameraKeys();
	SetupCamera();

    if (gScene && !bPause)
	{
		GetPhysicsResults();
        ProcessInputs();
		StartPhysics();
	}

    // Display scene
 	RenderActors(bShadows);
	gForceVec = NxVec3(0,0,0);

	// Render the HUD
	hud.Render();

    glFlush();
    glutSwapBuffers();
}

void ReshapeCallback(int width, int height)
{
    glViewport(0, 0, width, height);
    gCameraAspectRatio = float(width)/float(height);
}

void IdleCallback()
{
    glutPostRedisplay();
}

void KeyboardCallback(unsigned char key, int x, int y)
{
	gKeys[key] = true;

}

void KeyboardUpCallback(unsigned char key, int x, int y)
{
	gKeys[key] = false;

	switch (key)
	{

		case 27 : { exit(0); break; }
		default : { break; }
	}
}

void SpecialCallback(int key, int x, int y)
{
	switch (key)
    {
		// Reset PhysX
		case GLUT_KEY_F10: ResetNx(); return; 
	}
}

void MouseCallback(int button, int state, int x, int y)
{
    mx = x;
    my = y;
}

void MotionCallback(int x, int y)
{
    int dx = mx - x;
    int dy = my - y;
    
    gCameraForward.normalize();
    gCameraRight.cross(gCameraForward,NxVec3(0,1,0));

    NxQuat qx(NxPiF32 * dx * 20 / 180.0f, NxVec3(0,1,0));
    qx.rotate(gCameraForward);
    NxQuat qy(NxPiF32 * dy * 20 / 180.0f, gCameraRight);
    qy.rotate(gCameraForward);

    mx = x;
    my = y;
}

void ExitCallback()
{
	ReleaseNx();
}

void InitGlut(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(512, 512);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    gMainHandle = glutCreateWindow("Lesson 101: Primary Shape");
    glutSetWindow(gMainHandle);
    glutDisplayFunc(RenderCallback);
    glutReshapeFunc(ReshapeCallback);
    glutIdleFunc(IdleCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutKeyboardUpFunc(KeyboardUpCallback);
	glutSpecialFunc(SpecialCallback);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MotionCallback);
	MotionCallback(0,0);
	atexit(ExitCallback);

    // Setup default render states
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);

    // Setup lighting
    glEnable(GL_LIGHTING);
    float AmbientColor[]    = { 0.0f, 0.1f, 0.2f, 0.0f };         glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientColor);
    float DiffuseColor[]    = { 0.2f, 0.2f, 0.2f, 0.0f };         glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseColor);
    float SpecularColor[]   = { 0.5f, 0.5f, 0.5f, 0.0f };         glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularColor);
    float Position[]        = { 100.0f, 100.0f, -400.0f, 1.0f };  glLightfv(GL_LIGHT0, GL_POSITION, Position);
    glEnable(GL_LIGHT0);
}

NxActor* CreateGroundPlane()
{
    // Create a plane with default descriptor
    NxPlaneShapeDesc planeDesc;
    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&planeDesc);
    return gScene->createActor(actorDesc);
}

NxActor* CreateSphere()
{
	// Set the sphere starting height to 3.5m so box starts off falling onto the ground
	NxReal sphereStartHeight = 0;

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.flags |= NX_BF_KINEMATIC;
	// The actor has one shape, a sphere, 1m on radius
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius = 0.35f;
	sphereDesc.localPose.t = NxVec3(0, 0, 0);

	actorDesc.shapes.pushBack(&sphereDesc);
	actorDesc.body = &bodyDesc;
	actorDesc.density = 10.0f;
	actorDesc.globalPose.t = NxVec3(3.0f, sphereStartHeight, 0);
	return gScene->createActor(actorDesc);
}

void InitNx()
{
	// Initialize camera parameters
	gCameraAspectRatio	= 1.0f;
	gCameraPos			= NxVec3(-5,10,-5);
	gCameraForward		= NxVec3(0,0,1);
	gCameraRight		= NxVec3(-1,0,0);


	
	// Create the physics SDK
    gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
    if (!gPhysicsSDK)  return;

	// Set the physics parameters
	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.01);

	// Set the debug visualization parameters
	gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

    // Create the scene
    NxSceneDesc sceneDesc;
 	sceneDesc.simType				= NX_SIMULATION_SW;
    sceneDesc.gravity               = gDefaultGravity;
    gScene = gPhysicsSDK->createScene(sceneDesc);	
	if(!gScene)
	{ 
		sceneDesc.simType			= NX_SIMULATION_SW; 
		gScene = gPhysicsSDK->createScene(sceneDesc);  
		if(!gScene) return;
	}


	// Create the default material
	NxMaterial* defaultMaterial = gScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.5);
	defaultMaterial->setStaticFriction(0.5);
	defaultMaterial->setDynamicFriction(0.5);

	// Create the objects in the scene
	groundPlane		= CreateGroundPlane();

	

	// Get the current time
	getElapsedTime();

	// Start the first frame of the simulation
	if (gScene)  StartPhysics();


}

void ReleaseNx()
{
    if (gScene)
	{
		GetPhysicsResults();  // Make sure to fetchResults() before shutting down
		gPhysicsSDK->releaseScene(*gScene);
	}
	if (gPhysicsSDK)  gPhysicsSDK->release();
}

void ResetNx()
{
	ReleaseNx();
	InitNx();
}

void StartPhysics()
{
	// Update the time step
	gDeltaTime = getElapsedTime();
	// Start collision and dynamics for delta time since the last frame
    gScene->simulate(gDeltaTime);
	gScene->flushStream();
}

void GetPhysicsResults()
{
	// Get results from gScene->simulate(gDeltaTime)
	while (!gScene->fetchResults(NX_RIGID_BODY_FINISHED, false));
}

int main(int argc, char** argv)
{
	parser();
    InitGlut(argc, argv);
    InitNx();
	initite();
    glutMainLoop();
	ReleaseNx();
	return 0;
}
