#pragma once
#define CUBE_COLOR (0xFF00FFFF)

struct CUSTOMVERTEX
{
	FLOAT x, y, z;  // 12 bytes
	DWORD color; // 8 bytes
}; // 20 bytes

const CUSTOMVERTEX vs[8] = { 1.0f,1.0f,1.0f,CUBE_COLOR,
										1.0f,1.0f,-1.0f,CUBE_COLOR, 
										-1.0f,1.0f,-1.0f,CUBE_COLOR, 
										-1.0f,1.0f,1.0f,CUBE_COLOR, 
										1.0f,-1.0f,1.0f,CUBE_COLOR, 
										1.0f,-1.0f,-1.0f,CUBE_COLOR,
										-1.0f,-1.0f,-1.0f,CUBE_COLOR,
										-1.0f,-1.0f,1.0f,CUBE_COLOR};

const CUSTOMVERTEX cube_vertices[36] = { vs[3], vs[2], vs[1], vs[3], vs[2], vs[0], // up
								vs[7], vs[6], vs[2], vs[7], vs[2], vs[3], // left
								vs[0], vs[1], vs[5], vs[0], vs[5], vs[4], // right
								vs[2], vs[6], vs[5], vs[2], vs[5], vs[1], // front
								vs[7], vs[3], vs[0], vs[7], vs[0], vs[4], // back
								vs[4], vs[5], vs[6], vs[4], vs[6], vs[7], // down
}