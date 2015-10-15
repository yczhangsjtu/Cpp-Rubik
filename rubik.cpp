#include <vector>
#include <cmath>
#include <climits>
#include <iostream>
#include <GL/freeglut.h>


enum Axes {aX,aY,aZ};
enum Color {White,Yellow,Orange,Red,Green,Blue};
enum Facet {fDOWN,fUP,fLEFT,fRIGHT,fFRONT,fBACK,fNULL};


const double Pi = M_PI;



template<typename T>
std::vector<T> vec(T a1,T a2)
{
    T data[]= {a1,a2};
    std::vector<T> v(data,data+2);
    return v;
}

template<typename T>
std::vector<T> vec(T a1,T a2,T a3)
{
    T data[]= {a1,a2,a3};
    std::vector<T> v(data,data+3);
    return v;
}

template<typename T>
std::vector<T> vec(T a1,T a2,T a3,T a4)
{
    T data[]= {a1,a2,a3,a4};
    std::vector<T> v(data,data+4);
    return v;
}

template<typename T>
std::vector<T> vec(T a1,T a2,T a3,T a4,T a5,T a6)
{
    T data[]= {a1,a2,a3,a4,a5,a6};
    std::vector<T> v(data,data+6);
    return v;
}

template<typename T>
std::vector<T> vec(T a1,T a2,T a3,T a4,T a5,T a6,T a7,T a8)
{
    T data[]= {a1,a2,a3,a4,a5,a6,a7,a8};
    std::vector<T> v(data,data+8);
    return v;
}

template<typename T>
std::vector<T> vec(T a1,T a2,T a3,T a4,T a5,T a6,T a7,T a8,T a9,T a10,T a11,T a12)
{
    T data[]= {a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12};
    std::vector<T> v(data,data+12);
    return v;
}


void array3Step(Color arr[][3][3],const std::vector<std::vector<int> >& index)
{
    std::vector<Color> tmp=vec(	arr[index[0][0]][index[1][0]][index[2][0]],
                                arr[index[0][1]][index[1][1]][index[2][1]],
                                arr[index[0][2]][index[1][2]][index[2][2]]);
    for(int i=0; i<9; i++)
        arr[index[0][i]][index[1][i]][index[2][i]]=arr[index[0][i+3]][index[1][i+3]][index[2][i+3]];
    for(int i=9; i<12; i++)
        arr[index[0][i]][index[1][i]][index[2][i]]=tmp[i-9];
}

void array2Step(Color arr[][3][3],const std::vector<std::vector<int> >& index)
{
    std::vector<Color> tmp=vec(	arr[index[0][0]][index[1][0]][index[2][0]],
                                arr[index[0][1]][index[1][1]][index[2][1]]);
    for(int i=0; i<6; i++)
        arr[index[0][i]][index[1][i]][index[2][i]]=arr[index[0][i+2]][index[1][i+2]][index[2][i+2]];
    for(int i=6; i<8; i++)
        arr[index[0][i]][index[1][i]][index[2][i]]=tmp[i-6];
}

void drawMagicCube(GLenum mode);

typedef struct materialStruct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
} materialStruct;

void useMaterial(GLfloat R,GLfloat G,GLfloat B,GLfloat specular,GLfloat shrininess)
{
    materialStruct material = {
        {0.3*R, 0.3*G, 0.3*B, 1.0},
        {0.8*R, 0.8*G, 0.8*B, 1.0},
        {specular, specular, specular, 1.0},
        shrininess
    };
    glMaterialfv(GL_FRONT,GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,material.specular);
    glMaterialf(GL_FRONT,GL_SHININESS,material.shininess);
}

int getColor(Color color)
{
    switch(color)
    {
    case White:
        return 0xffffff;
    case Yellow:
        return 0xffff00;
    case Orange:
        return 0xff7700;
    case Red:
        return 0xff0000;
    case Green:
        return 0x00ff00;
    case Blue:
        return 0x0000ff;
    }
}

int ww=500;
int hh=500;

int scaleLeft=-100;
int scaleRight=100;
int scaleBottom=-100;
int scaleTop=100;
int scaleNear=200;
int scaleFar=1000;

int gridSize = 100;

double eyetheta = 2.5;
double eyedist = 700;
double eyeheight = 300;

const double flexi = 0.15;
const double minangle = 0.05;
const double rollSpeed = 2;

GLfloat shininess = 42.0;
GLfloat specular = 0.4;

void init()
{
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(scaleLeft,scaleRight,scaleBottom,scaleTop,scaleNear,scaleFar);
}

void reshape(int w, int h)
{
    ww = w;
    hh = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(ww<hh)
        glFrustum(scaleLeft,scaleRight,scaleBottom*(GLfloat)hh/(GLfloat)ww,
                  scaleTop*(GLfloat)hh/(GLfloat)ww,
                  scaleNear,scaleFar);
    else
        glFrustum(scaleLeft*(GLfloat)ww/(GLfloat)hh,
                  scaleRight*(GLfloat)ww/(GLfloat)hh,
                  scaleBottom,scaleTop,scaleNear,scaleFar);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,w,h);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyedist*cos(eyetheta),eyedist*sin(eyetheta),eyeheight,0,0,0,0,0,1);
    GLfloat light0_pos[4]= {eyedist*cos(eyetheta),eyedist*sin(eyetheta),
                            eyeheight,1.0
                           };
    GLfloat light0_spt[4]= {-eyedist*cos(eyetheta),-eyedist*sin(eyetheta),
                            -eyeheight,1.0
                           };
    glLightfv(GL_LIGHT0,GL_POSITION,light0_pos);
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_spt);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,45);
    drawMagicCube(GL_RENDER);
    glutSwapBuffers();
}

void crossProduct(GLfloat v1[],GLfloat v2[],GLfloat r[])
{
    r[0] = v1[1]*v2[2]-v1[2]*v2[1];
    r[1] = v1[2]*v2[0]-v1[0]*v2[2];
    r[2] = v1[0]*v2[1]-v1[1]*v2[0];
}


void drawSquare(GLfloat * p1,GLfloat * p2,GLfloat * p3,GLfloat * p4,
                unsigned int color,int id,GLenum mode)
{
    if(mode==GL_SELECT) glLoadName(id);
    GLfloat r = ((color>>16)%256)/255.0;
    GLfloat g = ((color>>8)%256)/255.0;
    GLfloat b = (color%256)/255.0;
    GLfloat v1[3]= {p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2]};
    GLfloat v2[3]= {p3[0]-p2[0],p3[1]-p2[1],p3[2]-p2[2]};
    GLfloat normal[3];
    crossProduct(v1,v2,normal);
    GLfloat d = sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
    for(int i=0; i<3; i++) normal[i]/=d;
    glNormal3fv(normal);
    //glColor3f(r,g,b);
    useMaterial(r,g,b,specular,shininess);
    glPolygonMode(GL_FRONT,GL_FILL);
    glBegin(GL_POLYGON);
    glVertex3fv(p1);
    glVertex3fv(p2);
    glVertex3fv(p3);
    glVertex3fv(p4);
    glEnd();
    glPolygonMode(GL_FRONT,GL_LINE);
    useMaterial(r,g,b,0,0);
    glBegin(GL_POLYGON);
    glVertex3fv(p1);
    glVertex3fv(p2);
    glVertex3fv(p3);
    glVertex3fv(p4);
    glEnd();
}

Color initialColorMap[6][3][3];

class MagicCube
{
public:
    MagicCube(Color colorMap[][3][3])
        :_axes(aX),_rotation(vec(0.0,0.0,0.0))
    {
        for(int i=0; i<6; i++)
            for(int j=0; j<3; j++)
                for(int k=0; k<3; k++)
                    _colors[i][j][k]=colorMap[i][j][k];
    }
    void rotateColor(Axes axes,int layer,bool direction)
    {
        switch(axes)
        {
        case aX:
            switch(layer)
            {
            case 0:
                if(direction)
                {
                    array2Step(_colors,vec(vec(4,4,4,4,4,4,4,4),
                                           vec(1,2,2,2,1,0,0,0),
                                           vec(2,2,1,0,0,0,1,2)));
                    array3Step(_colors,vec(vec(1,1,1,3,3,3,0,0,0,2,2,2),
                                           vec(0,0,0,0,0,0,0,0,0,0,0,0),
                                           vec(2,1,0,0,1,2,0,1,2,2,1,0)));
                }
                else
                {
                    array2Step(_colors,vec(vec(4,4,4,4,4,4,4,4),
                                           vec(0,0,0,1,2,2,2,1),
                                           vec(2,1,0,0,0,1,2,2)));
                    array3Step(_colors,vec(vec(2,2,2,0,0,0,3,3,3,1,1,1),
                                           vec(0,0,0,0,0,0,0,0,0,0,0,0),
                                           vec(0,1,2,2,1,0,2,1,0,0,1,2)));
                }
                break;
            case 1:
                if(direction)
                {
                    array3Step(_colors,vec(vec(1,1,1,3,3,3,0,0,0,2,2,2),
                                           vec(1,1,1,1,1,1,1,1,1,1,1,1),
                                           vec(0,1,2,2,1,0,2,1,0,0,1,2)));
                }
                else
                {
                    array3Step(_colors,vec(vec(2,2,2,0,0,0,3,3,3,1,1,1),
                                           vec(1,1,1,1,1,1,1,1,1,1,1,1),
                                           vec(2,1,0,0,1,2,0,1,2,2,1,0)));
                }
                break;
            case 2:
                if(direction)
                {
                    array2Step(_colors,vec(vec(5,5,5,5,5,5,5,5),
                                           vec(1,2,2,2,1,0,0,0),
                                           vec(2,2,1,0,0,0,1,2)));
                    array3Step(_colors,vec(vec(1,1,1,3,3,3,0,0,0,2,2,2),
                                           vec(2,2,2,2,2,2,2,2,2,2,2,2),
                                           vec(2,1,0,0,1,2,0,1,2,2,1,0)));
                }
                else
                {
                    array2Step(_colors,vec(vec(5,5,5,5,5,5,5,5),
                                           vec(0,0,0,1,2,2,2,1),
                                           vec(2,1,0,0,0,1,2,2)));
                    array3Step(_colors,vec(vec(2,2,2,0,0,0,3,3,3,1,1,1),
                                           vec(2,2,2,2,2,2,2,2,2,2,2,2),
                                           vec(0,1,2,2,1,0,2,1,0,0,1,2)));
                }
                break;
            }
            break;
        case aY:
            switch(layer)
            {
            case 0:
                if(direction)
                {
                    array2Step(_colors,vec(vec(2,2,2,2,2,2,2,2),
                                           vec(1,2,2,2,1,0,0,0),
                                           vec(2,2,1,0,0,0,1,2)));
                    array3Step(_colors,vec(vec(1,1,1,5,5,5,0,0,0,4,4,4),
                                           vec(2,1,0,0,0,0,0,1,2,0,0,0),
                                           vec(0,0,0,0,1,2,0,0,0,2,1,0)));
                }
                else
                {
                    array2Step(_colors,vec(vec(2,2,2,2,2,2,2,2),
                                           vec(0,0,0,1,2,2,2,1),
                                           vec(2,1,0,0,0,1,2,2)));
                    array3Step(_colors,vec(vec(4,4,4,0,0,0,5,5,5,1,1,1),
                                           vec(0,0,0,2,1,0,0,0,0,0,1,2),
                                           vec(0,1,2,0,0,0,2,1,0,0,0,0)));
                }
                break;
            case 1:
                if(direction)
                {
                    array3Step(_colors,vec(vec(1,1,1,5,5,5,0,0,0,4,4,4),
                                           vec(2,1,0,1,1,1,0,1,2,1,1,1),
                                           vec(1,1,1,0,1,2,1,1,1,2,1,0)));
                }
                else
                {
                    array3Step(_colors,vec(vec(4,4,4,0,0,0,5,5,5,1,1,1),
                                           vec(1,1,1,2,1,0,1,1,1,0,1,2),
                                           vec(0,1,2,1,1,1,2,1,0,1,1,1)));
                }
                break;
            case 2:
                if(direction)
                {
                    array2Step(_colors,vec(vec(3,3,3,3,3,3,3,3),
                                           vec(1,2,2,2,1,0,0,0),
                                           vec(2,2,1,0,0,0,1,2)));
                    array3Step(_colors,vec(vec(1,1,1,5,5,5,0,0,0,4,4,4),
                                           vec(2,1,0,2,2,2,0,1,2,2,2,2),
                                           vec(2,2,2,0,1,2,2,2,2,2,1,0)));
                }
                else
                {
                    array2Step(_colors,vec(vec(3,3,3,3,3,3,3,3),
                                           vec(0,0,0,1,2,2,2,1),
                                           vec(2,1,0,0,0,1,2,2)));
                    array3Step(_colors,vec(vec(4,4,4,0,0,0,5,5,5,1,1,1),
                                           vec(2,2,2,2,1,0,2,2,2,0,1,2),
                                           vec(0,1,2,2,2,2,2,1,0,2,2,2)));
                }
                break;
            }
            break;
        case aZ:
            switch(layer)
            {
            case 0:
                if(direction)
                {
                    array2Step(_colors,vec(vec(0,0,0,0,0,0,0,0),
                                           vec(1,2,2,2,1,0,0,0),
                                           vec(2,2,1,0,0,0,1,2)));
                    array3Step(_colors,vec(vec(2,2,2,4,4,4,3,3,3,5,5,5),
                                           vec(0,1,2,2,1,0,2,1,0,0,1,2),
                                           vec(0,0,0,0,0,0,0,0,0,0,0,0)));
                }
                else
                {
                    array2Step(_colors,vec(vec(0,0,0,0,0,0,0,0),
                                           vec(0,0,0,1,2,2,2,1),
                                           vec(2,1,0,0,0,1,2,2)));
                    array3Step(_colors,vec(vec(5,5,5,3,3,3,4,4,4,2,2,2),
                                           vec(2,1,0,0,1,2,0,1,2,2,1,0),
                                           vec(0,0,0,0,0,0,0,0,0,0,0,0)));
                }
                break;
            case 1:
                if(direction)
                {
                    array3Step(_colors,vec(vec(2,2,2,4,4,4,3,3,3,5,5,5),
                                           vec(0,1,2,2,1,0,2,1,0,0,1,2),
                                           vec(1,1,1,1,1,1,1,1,1,1,1,1)));
                }
                else
                {
                    array3Step(_colors,vec(vec(5,5,5,3,3,3,4,4,4,2,2,2),
                                           vec(2,1,0,0,1,2,0,1,2,2,1,0),
                                           vec(1,1,1,1,1,1,1,1,1,1,1,1)));
                }
                break;
            case 2:
                if(direction)
                {
                    array2Step(_colors,vec(vec(1,1,1,1,1,1,1,1),
                                           vec(1,2,2,2,1,0,0,0),
                                           vec(2,2,1,0,0,0,1,2)));
                    array3Step(_colors,vec(vec(2,2,2,4,4,4,3,3,3,5,5,5),
                                           vec(0,1,2,2,1,0,2,1,0,0,1,2),
                                           vec(2,2,2,2,2,2,2,2,2,2,2,2)));
                }
                else
                {
                    array2Step(_colors,vec(vec(1,1,1,1,1,1,1,1),
                                           vec(0,0,0,1,2,2,2,1),
                                           vec(2,1,0,0,0,1,2,2)));
                    array3Step(_colors,vec(vec(5,5,5,3,3,3,4,4,4,2,2,2),
                                           vec(2,1,0,0,1,2,0,1,2,2,1,0),
                                           vec(2,2,2,2,2,2,2,2,2,2,2,2)));
                }
                break;
            }
            break;
        }
    }
    bool noRotation()
    {
        double eps=flexi;
        return (std::fabs(_rotation[0])<eps &&
                std::fabs(_rotation[1])<eps &&
                std::fabs(_rotation[2])<eps);
    }
    int rotate(Axes axes,int layer,double rotation)
    {
        if(axes!=_axes && !noRotation())
            return -1;
        if(axes!=_axes)
            _rotation=vec(0.0,0.0,0.0);
        _rotation[layer]+=rotation;
        _axes=axes;
        if(_rotation[layer]<-Pi/4)
        {
            _rotation[layer]+=Pi/2;
            rotateColor(axes,layer,false);
            return 1;
        }
        else if(_rotation[layer]>=Pi/4)
        {
            _rotation[layer]-=Pi/2;
            rotateColor(axes,layer,true);
            return 1;
        }
        if(fabs(_rotation[layer])<minangle)
            _rotation[layer]=0;
        return 0;
    }
    void draw(GLenum mode)
    {
        GLfloat points[3][3][3][2][2][2][3];
        for(int i = 0; i<3; i++)
            for(int j = 0; j<3; j++)
                for(int k = 0; k<3; k++)
                    for(int x = 0; x<2; x++)
                        for(int y=0; y<2; y++)
                            for(int z=0; z<2; z++)
                            {
                                points[i][j][k][x][y][z][0]=(i+x) * gridSize - gridSize/2*3;
                                points[i][j][k][x][y][z][1]=(j+y) * gridSize - gridSize/2*3;
                                points[i][j][k][x][y][z][2]=(k+z) * gridSize - gridSize/2*3;
                            }
        switch(_axes)
        {
        case aX:
            for(int i = 0; i<3; i++)
                for(int j = 0; j<3; j++)
                    for(int k = 0; k<3; k++)
                        for(int x = 0; x<2; x++)
                            for(int y=0; y<2; y++)
                                for(int z=0; z<2; z++)
                                {
                                    GLfloat oy = points[i][j][k][x][y][z][1];
                                    GLfloat oz = points[i][j][k][x][y][z][2];
                                    GLfloat ny,nz;
                                    ny = oy * cos(_rotation[i])
                                         -oz * sin(_rotation[i]);
                                    nz = oy * sin(_rotation[i])
                                         +oz * cos(_rotation[i]);
                                    points[i][j][k][x][y][z][1] = ny;
                                    points[i][j][k][x][y][z][2] = nz;
                                }
            break;
        case aY:
            for(int i = 0; i<3; i++)
                for(int j = 0; j<3; j++)
                    for(int k = 0; k<3; k++)
                        for(int x = 0; x<2; x++)
                            for(int y=0; y<2; y++)
                                for(int z=0; z<2; z++)
                                {
                                    GLfloat ox = points[i][j][k][x][y][z][0];
                                    GLfloat oz = points[i][j][k][x][y][z][2];
                                    GLfloat nx, nz;
                                    nx = ox * cos(_rotation[j])
                                         -oz * sin(_rotation[j]);
                                    nz = ox * sin(_rotation[j])
                                         +oz * cos(_rotation[j]);
                                    points[i][j][k][x][y][z][0] = nx;
                                    points[i][j][k][x][y][z][2] = nz;
                                }
            break;
        case aZ:
            for(int i = 0; i<3; i++)
                for(int j = 0; j<3; j++)
                    for(int k = 0; k<3; k++)
                        for(int x = 0; x<2; x++)
                            for(int y=0; y<2; y++)
                                for(int z=0; z<2; z++)
                                {
                                    GLfloat ox = points[i][j][k][x][y][z][0];
                                    GLfloat oy = points[i][j][k][x][y][z][1];
                                    GLfloat nx = ox * cos(_rotation[k])
                                                 - oy * sin(_rotation[k]);
                                    GLfloat ny = ox * sin(_rotation[k])
                                                 + oy * cos(_rotation[k]);
                                    points[i][j][k][x][y][z][0] = nx;
                                    points[i][j][k][x][y][z][1] = ny;
                                }
            break;
        }
        for(int i = 0; i<3; i++)
            for(int j = 0; j<3; j++)
                for(int k = 0; k<3; k++)
                {
                    GLfloat normals[3];
                    Color color = White;
                    if(k==0) color = _colors[0][i][j];
                    drawSquare(	points[i][j][k][0][0][0],points[i][j][k][0][1][0],
                                points[i][j][k][1][1][0],points[i][j][k][1][0][0],
                                getColor(color),((2-i)*9+j*3+k)*6+0,mode);
                    color = White;
                    if(k==2) color = _colors[1][i][j];
                    drawSquare(	points[i][j][k][0][0][1],points[i][j][k][1][0][1],
                                points[i][j][k][1][1][1],points[i][j][k][0][1][1],
                                getColor(color),((2-i)*9+j*3+k)*6+1,mode);
                    color = White;
                    if(j==0) color = _colors[2][i][k];
                    drawSquare(	points[i][j][k][0][0][0],points[i][j][k][1][0][0],
                                points[i][j][k][1][0][1],points[i][j][k][0][0][1],
                                getColor(color),((2-i)*9+j*3+k)*6+3,mode);
                    color = White;
                    if(j==2) color = _colors[3][i][k];
                    drawSquare(	points[i][j][k][0][1][0],points[i][j][k][0][1][1],
                                points[i][j][k][1][1][1],points[i][j][k][1][1][0],
                                getColor(color),((2-i)*9+j*3+k)*6+2,mode);
                    color = White;
                    if(i==0) color = _colors[4][j][k];
                    drawSquare(	points[i][j][k][0][0][0],points[i][j][k][0][0][1],
                                points[i][j][k][0][1][1],points[i][j][k][0][1][0],
                                getColor(color),((2-i)*9+j*3+k)*6+4,mode);
                    color = White;
                    if(i==2) color = _colors[5][j][k];
                    drawSquare(	points[i][j][k][1][0][0],points[i][j][k][1][1][0],
                                points[i][j][k][1][1][1],points[i][j][k][1][0][1],
                                getColor(color),((2-i)*9+j*3+k)*6+5,mode);
                }
    }
    std::vector<double> _rotation;
    Axes _axes;
private:
    Color _colors[6][3][3];
};

MagicCube magicCube(initialColorMap);

void drawMagicCube(GLenum mode)
{
    magicCube.draw(mode);
}

void initialize()
{
    for(int i=0; i<6; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                initialColorMap[i][j][k]=(Color)i;
    magicCube=MagicCube(initialColorMap);
}

void menu(int value)
{
    switch(value)
    {
    case 0:
        initialize();
        break;
    case 1:
        exit(0);
        break;
    }
    glutPostRedisplay();
}

const int sbSize = 8192;

int selected = -1;
int mousex, mousey;


void handleMouseEvent(int button, int state, int x, int y)
{
    if(state==GLUT_DOWN)
    {
        if(button == GLUT_LEFT_BUTTON)
        {
            GLint hits;
            GLuint selectBuf[sbSize];
            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT,viewport);
            glSelectBuffer(sbSize,selectBuf);
            glRenderMode(GL_SELECT);
            glInitNames();
            glPushName(0);
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            gluPickMatrix ((GLdouble) x, (GLdouble)(viewport[3]- y),
                           0.1, 0.1, viewport);
            if(ww<hh)
                glFrustum(scaleLeft,scaleRight,scaleBottom*(GLfloat)hh/(GLfloat)ww,
                          scaleTop*(GLfloat)hh/(GLfloat)ww,
                          scaleNear,scaleFar);
            else
                glFrustum(scaleLeft*(GLfloat)ww/(GLfloat)hh,scaleRight*(GLfloat)ww/(GLfloat)hh,
                          scaleBottom,scaleTop,scaleNear,scaleFar);
            magicCube.draw(GL_SELECT);
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glFlush();
            hits=glRenderMode(GL_RENDER);
            if(hits>0)
            {
                GLint names, *ptr;
                GLint depth=INT_MAX;
                ptr = (GLint *) selectBuf;
                for (int i = 0; i < hits; i++)
                {
                    names = *ptr;
                    GLint minDepth = ptr[1];
                    GLint maxDepth = ptr[2];
                    ptr+=3;
                    for (int j = 0; j < names; j++)
                    {
                        if(maxDepth<depth)
                        {
                            selected = *ptr;
                            depth = maxDepth;
                        }
                        ptr++;
                    }
                }
            }
            else
            {
                selected = -1;
            }
        }
        else
            selected = -1;
    }
    else
    {
        selected = -1;
    }
    mousex = x;
    mousey = y;
}

void handleMouseMoveEvent(int x, int y)
{
    int sum = 0;
    int sx, sy, sz, f;
    double dx=x-mousex;
    double dy=y-mousey;
    if(selected >= 0)
    {
        f = selected % 6;
        sz = (selected / 6) % 3;
        sy = (selected / 18) % 3;
        sx = (selected / 54) % 3;
        if(sx==1) sum++;
        if(sy==1) sum++;
        if(sz==1) sum++;
    }
    if(selected<0)
    {
        double dx=x-mousex;
        double dy=y-mousey;
        eyetheta -= dx/100.0;
        if(eyetheta > 2*M_PI) eyetheta -= 2*M_PI;
        if(eyetheta < 0) eyetheta += 2*M_PI;
        eyeheight += rollSpeed*dy;
        if(eyeheight > 600) eyeheight = 600;
        if(eyeheight < -600) eyeheight = -600;
    }
    else if(sum==1)
    {
        std::vector<int> targetCubic = vec(sx,sy,sz);
        Axes axes;
        int index=-1;
        if(	(targetCubic==vec(2,0,1) && (Facet)f==fRIGHT) ||
                (targetCubic==vec(2,1,0) && (Facet)f==fDOWN ) ||
                (targetCubic==vec(2,2,1) && (Facet)f==fLEFT ) ||
                (targetCubic==vec(2,1,2) && (Facet)f==fUP   ) )
        {
            if(magicCube._axes==aX || magicCube.noRotation())
            {
                axes=aX;
                index=0;
            }
        }
        else if((targetCubic==vec(0,0,1) && (Facet)f==fRIGHT) ||
                (targetCubic==vec(0,1,0) && (Facet)f==fDOWN ) ||
                (targetCubic==vec(0,2,1) && (Facet)f==fLEFT ) ||
                (targetCubic==vec(0,1,2) && (Facet)f==fUP   ))
        {
            if(magicCube._axes==aX || magicCube.noRotation())
            {
                axes=aX;
                index=2;
            }
        }
        else if
        ((targetCubic==vec(1,0,0) && (Facet)f==fDOWN ) ||
                (targetCubic==vec(0,0,1) && (Facet)f==fBACK ) ||
                (targetCubic==vec(1,0,2) && (Facet)f==fUP   ) ||
                (targetCubic==vec(2,0,1) && (Facet)f==fFRONT) )
        {
            if(magicCube._axes==aY || magicCube.noRotation())
            {
                axes=aY;
                index=0;
            }
        }
        else if((targetCubic==vec(1,2,0) && (Facet)f==fDOWN ) ||
                (targetCubic==vec(0,2,1) && (Facet)f==fBACK ) ||
                (targetCubic==vec(1,2,2) && (Facet)f==fUP   ) ||
                (targetCubic==vec(2,2,1) && (Facet)f==fFRONT))
        {
            if(magicCube._axes==aY || magicCube.noRotation())
            {
                axes=aY;
                index=2;
            }
        }
        else if
        ((targetCubic==vec(1,0,0) && (Facet)f==fRIGHT) ||
                (targetCubic==vec(0,1,0) && (Facet)f==fBACK ) ||
                (targetCubic==vec(1,2,0) && (Facet)f==fLEFT ) ||
                (targetCubic==vec(2,1,0) && (Facet)f==fFRONT) )
        {
            if(magicCube._axes==aZ || magicCube.noRotation())
            {
                axes=aZ;
                index=0;
            }
        }
        else if((targetCubic==vec(1,0,2) && (Facet)f==fRIGHT) ||
                (targetCubic==vec(0,1,2) && (Facet)f==fBACK ) ||
                (targetCubic==vec(1,2,2) && (Facet)f==fLEFT ) ||
                (targetCubic==vec(2,1,2) && (Facet)f==fFRONT))
        {
            if(magicCube._axes==aZ || magicCube.noRotation())
            {
                axes=aZ;
                index=2;
            }
        }
        else
        {
            selected = -1;
            return;
        }
        if(index<0)
        {
            selected = -1;
            return;
        }
        switch(axes)
        {
        case aX:
            if(f==(int)fRIGHT)
                magicCube.rotate(axes,index,dy/50);
            else if(f==(int)fLEFT)
                magicCube.rotate(axes,index,-dy/50);
            else if(f==(int)fDOWN)
            {
                if(eyetheta<M_PI/4 || eyetheta>7*M_PI/4)
                    magicCube.rotate(axes,index,dx/50);
                else if(eyetheta<3*M_PI/4 && eyetheta>M_PI/4)
                    magicCube.rotate(axes,index,-dy/50);
                else if(eyetheta<5*M_PI/4 && eyetheta>3*M_PI/4)
                    magicCube.rotate(axes,index,-dx/50);
                else if(eyetheta<7*M_PI/4 && eyetheta>5*M_PI/4)
                    magicCube.rotate(axes,index,dy/50);
            }
            else if(f==(int)fUP)
            {
                if(eyetheta<M_PI/4 || eyetheta>7*M_PI/4)
                    magicCube.rotate(axes,index,-dx/50);
                else if(eyetheta<3*M_PI/4 && eyetheta>M_PI/4)
                    magicCube.rotate(axes,index,-dy/50);
                else if(eyetheta<5*M_PI/4 && eyetheta>3*M_PI/4)
                    magicCube.rotate(axes,index,dx/50);
                else if(eyetheta<7*M_PI/4 && eyetheta>5*M_PI/4)
                    magicCube.rotate(axes,index,dy/50);
            }
            else
                magicCube.rotate(axes,index,dy/50);
            break;
        case aY:
            if(f==(int)fFRONT)
                magicCube.rotate(axes,index,dy/50);
            else if(f==(int)fBACK)
                magicCube.rotate(axes,index,-dy/50);
            else if(f==(int)fDOWN)
            {
                if(eyetheta<M_PI/4 || eyetheta>7*M_PI/4)
                    magicCube.rotate(axes,index,-dy/50);
                else if(eyetheta<3*M_PI/4 && eyetheta>M_PI/4)
                    magicCube.rotate(axes,index,-dx/50);
                else if(eyetheta<5*M_PI/4 && eyetheta>3*M_PI/4)
                    magicCube.rotate(axes,index,dy/50);
                else if(eyetheta<7*M_PI/4 && eyetheta>5*M_PI/4)
                    magicCube.rotate(axes,index,dx/50);
            }
            else if(f==(int)fUP)
            {
                if(eyetheta<M_PI/4 || eyetheta>7*M_PI/4)
                    magicCube.rotate(axes,index,-dy/50);
                else if(eyetheta<3*M_PI/4 && eyetheta>M_PI/4)
                    magicCube.rotate(axes,index,dx/50);
                else if(eyetheta<5*M_PI/4 && eyetheta>3*M_PI/4)
                    magicCube.rotate(axes,index,dy/50);
                else if(eyetheta<7*M_PI/4 && eyetheta>5*M_PI/4)
                    magicCube.rotate(axes,index,-dx/50);
            }
            else magicCube.rotate(axes,index,dy/50);
            break;
        case aZ:
            magicCube.rotate(axes,index,dx/50);
            break;
        }
    }
    else
        selected = -1;
    mousex = x;
    mousey = y;
    glutPostRedisplay();
}

void fp()
{
    magicCube.rotate(aX,0,0.2);
}
void bp()
{
    magicCube.rotate(aX,2,-0.2);
}
void lp()
{
    magicCube.rotate(aY,2,0.2);
}
void rp()
{
    magicCube.rotate(aY,0,-0.2);
}
void dp()
{
    magicCube.rotate(aZ,0,+0.2);
}
void up()
{
    magicCube.rotate(aZ,2,-0.2);
}
void fn()
{
    magicCube.rotate(aX,0,-0.2);
}
void bn()
{
    magicCube.rotate(aX,2,0.2);
}
void ln()
{
    magicCube.rotate(aY,2,-0.2);
}
void rn()
{
    magicCube.rotate(aY,0,0.2);
}
void dn()
{
    magicCube.rotate(aZ,0,-0.2);
}
void un()
{
    magicCube.rotate(aZ,2,0.2);
}


bool positive=true;
void handleKeyEvent(unsigned char keyCode, int x, int y)
{
    if(keyCode=='c') initialize();
    if(keyCode=='p') positive=true;
    if(keyCode=='n') positive=false;
    if(keyCode=='d') dp();
    if(keyCode=='D') dn();
    if(keyCode=='u') up();
    if(keyCode=='U') un();
    if(keyCode=='l') lp();
    if(keyCode=='L') ln();
    if(keyCode=='r') rp();
    if(keyCode=='R') rn();
    if(keyCode=='f') fp();
    if(keyCode=='F') fn();
    if(keyCode=='b') bp();
    if(keyCode=='B') bn();
    if(keyCode=='q' && glutGetModifiers()==GLUT_ACTIVE_ALT) exit(0);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    initialize();
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(ww,hh);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Magic Cube");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyEvent);
    glutMouseFunc(handleMouseEvent);
    glutMotionFunc(handleMouseMoveEvent);
    init();
    int id = glutCreateMenu(menu);
    glutSetMenu(id);
    glutAddMenuEntry("Recover",0);
    glutAddMenuEntry("Exit",1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
}
