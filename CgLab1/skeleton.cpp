// Introduction lab that covers:
// * C++
// * SDL
// * 2D graphics
// * Plotting pixels
// * Video memory
// * Color representation
// * Linear interpolation
// * glm::vec3 and std::vector

#include "SDL.h"
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "SDLauxiliary.h"

using namespace std;
using glm::vec3;
using glm::vec2;

// --------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
int t;
float f = SCREEN_HEIGHT /2;
vector<vec3> stars(1000);
vector<vec2> projected_stars(1000);
SDL_Surface* screen;

// --------------------------------------------------------
// FUNCTION DECLARATIONS

void Draw();
void Draw2();
void Update();
void drawStars();
void Interpolate(float, float, vector<float>&);
void Interpolate(vec3, vec3, vector<vec3>&);


// --------------------------------------------------------
// FUNCTION DEFINITIONS

int main( int argc, char* argv[] )
{
	screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT );
	drawStars();
	t = SDL_GetTicks();
	while( NoQuitMessageSDL() )
	{	
		Update();
		Draw2();
	}
	SDL_SaveBMP(screen, "screenshot.bmp" );


/**
	vector<float> result(10); // Create a vector width 10 floats
	Interpolate( 5, 14, result); // Fill it with interpolated values
	for(int i=0; i<result.size(); ++i)
	{	
		cout << result[i] << " "; // Print the result to the terminal
	} 
	
	vector<vec3> result( 4 );
	vec3 a(1,4,9.2);
	vec3 b(4,1,9.8);
	Interpolate(a, b, result);
	for( int i=0; i<result.size(); ++i ){
		cout << "( "<< result[i].x << ", "<< result[i].y << ", "<< result[i].z << " ) ";
		}
		**/

	return 0;
}

void drawStars(){

	for(int i= 0; i< stars.size() ; i++)
	{
		//calculate star positions
		stars[i].x = 2*(float(rand()) / (float)RAND_MAX - 0.5);
		stars[i].y = 2*(float(rand()) / (float)RAND_MAX - 0.5);
		stars[i].z = 2*(float(rand()) / (float)RAND_MAX - 0.5);

		//calculate projected stars
		projected_stars[i].x = f*(stars[i].x/stars[i].z) + SCREEN_WIDTH/2;
		projected_stars[i].y = f*(stars[i].y/stars[i].z) + SCREEN_HEIGHT/2;
	}
}

void Draw2(){
	SDL_FillRect( screen, 0, 0 );
	if( SDL_MUSTLOCK(screen) ){
		SDL_LockSurface(screen);
	}
	vec3 color_white(1,1,1);
	for( int y=0; y < projected_stars.size(); ++y )
	{	
	PutPixelSDL(screen, projected_stars[y].x, projected_stars[y].y, color_white);
	}

	if( SDL_MUSTLOCK(screen) ){
		SDL_UnlockSurface(screen);
	}
	SDL_UpdateRect( screen, 0, 0, 0, 0 );
}
void Update(){
	int t2 = SDL_GetTicks();
	float dt = float(t2-t);
	t = t2;
	for( int s=0; s<stars.size(); ++s ){
		// Add code for update of stars
		stars[s].z = stars[s].z - 0.001*dt;

		if( stars[s].z <= 0 )
		{ 
			stars[s].z += 1;
		}
		if( stars[s].z > 1 )
		{
			stars[s].z -= 1;
		}
		projected_stars[s].x = f*(stars[s].x/stars[s].z) + SCREEN_WIDTH/2;
		projected_stars[s].y = f*(stars[s].y/stars[s].z) + SCREEN_HEIGHT/2;

	}

}

void Draw(){	
	vec3 topLeft(1,0,0); // red 
	vec3 topRight(0,0,1); // blue
	vec3 bottomLeft(0,1,0); // green
	vec3 bottomRight(1,1,0); // yellow

	vector<vec3> leftSide( SCREEN_HEIGHT );
	vector<vec3> rightSide( SCREEN_HEIGHT );
	Interpolate( topLeft, bottomLeft, leftSide);
	Interpolate( topRight, bottomRight, rightSide);

	
	for( int y=0; y<SCREEN_HEIGHT; ++y )
	{	
		vector<vec3> colors (SCREEN_WIDTH);
		Interpolate(leftSide[y], rightSide[y], colors);
		for( int x=0; x<SCREEN_WIDTH; ++x )
		{	
			//vec3 color(1,0,1);
			PutPixelSDL(screen, x, y, colors[x]);

		}
	}

	if( SDL_MUSTLOCK(screen) )
		SDL_UnlockSurface(screen);

	SDL_UpdateRect( screen, 0, 0, 0, 0 );
}

void Interpolate( float a, float b, vector<float>& result){
	float c, increment; 
	c = (b - a);
	if (result.size() <= 1){
		result[0] = c/2;
		return;
		}
	// cout << "resultsize: " << result.size() << " " << c;
	increment = (c+1)/(result.size());
	// cout << "increment: " << increment << " ";
	for(int i = 0; i < result.size(); i++){
		result[i] = a+(increment*i);
	}
}

void Interpolate( vec3 a, vec3 b, vector<vec3>& result){
	vec3 c;
	c.x = b.x-a.x;
	
	c.y = b.y-a.y;
	c.z = b.z-a.z;
	float incx, incy, incz;
	if (result.size() <= 1){
		result[0].x = a.x + c.x/2;
		result[0].y = a.y + c.y/2;
		result[0].z = a.z + c.z/2;
		return;
		}
	incx = (c.x) / (result.size()-1);
	incy = (c.y) / (result.size()-1);
	incz = (c.z) / (result.size()-1);
	for(int i = 0; i < result.size(); i++){
		result[i].x = a.x+incx*i;
		result[i].y = a.y+incy*i;
		result[i].z = a.z+incz*i;
	}

}



