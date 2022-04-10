// 3DGraphicsEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "olcConsoleGameEngine.h"

struct vec3d
{
    float x, y, z;
};

struct triangle
{
    vec3d p[3];
};

struct mesh
{
    std::vector<triangle> tris;
};

struct mat4x4
{
    float m[4][4] = { 0 };
};

class olcEngine3D : public olcConsoleGameEngine
{
public:
    olcEngine3D()
    {
        m_sAppName = L"3D Demo";
    }

private:
    mesh meshCube;
    mat4x4 matProj;

    void MatrixMultiplyVector(vec3d& i, vec3d& o, mat4x4& m)
    {
        o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
        o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
        o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
        float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

        if (w != 0.0f)
        {
            o.x /= w;
            o.y /= w;
            o.z /= w;
        }
    }



public:
    bool OnUserCreate() override
    {
        meshCube.tris =
        {
            { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

                                                        
            { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },
                                                  
            { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

                                                      
            { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },
                                                      
            { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

                                                      
            { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
        };


        float fNear = 0.1f;
        float fFar = 1000.0f;
        float fFov = 90.0f;
        float fAspRatio = (float)ScreenHeight() / (float)ScreenWidth();
        float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

        matProj.m[0][0] = fAspRatio * fFovRad;
        matProj.m[1][1] = fFovRad;
        matProj.m[2][2] = fFar / (fFar - fNear);
        matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
        matProj.m[2][3] = 1.0f;
        matProj.m[3][3] = 0.0f;
        return true;
    }

public:
    bool OnUserUpdate(float fElapsedTime) override
    {
        Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

        //Draw Triangles:
        for (auto tri : meshCube.tris)
        {
            triangle triProj;
            MatrixMultiplyVector(tri.p[0], triProj.p[0], matProj);
            MatrixMultiplyVector(tri.p[1], triProj.p[1], matProj);
            MatrixMultiplyVector(tri.p[2], triProj.p[2], matProj);


            triProj.p[0].x += 1.0f; triProj.p[0].y += 1.0f;
            triProj.p[1].x += 1.0f; triProj.p[1].y += 1.0f;
            triProj.p[2].x += 1.0f; triProj.p[2].y += 1.0f;

            triProj.p[0].x *= 0.5f * (float)ScreenWidth();
            triProj.p[0].y *= 0.5f * (float)ScreenHeight();
            triProj.p[1].x *= 0.5f * (float)ScreenWidth();
            triProj.p[1].y *= 0.5f * (float)ScreenHeight();
            triProj.p[2].x *= 0.5f * (float)ScreenWidth();
            triProj.p[2].y *= 0.5f * (float)ScreenHeight();

            DrawTriangle(triProj.p[0].x, triProj.p[0].y,
                triProj.p[1].x, triProj.p[1].y,
                triProj.p[2].x, triProj.p[2].y,
                PIXEL_SOLID, FG_WHITE);
        }
        return true;
    }
};



int main()
{
    olcEngine3D demo;
    if (demo.ConstructConsole(256, 240, 4, 4))
        demo.Start();
    return 0;
}