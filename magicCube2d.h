#ifndef _MAGICCUBE_H
#define _MAGICCUBE_H

#include <string>
#include <vector>
#include <cmath>
#include <GL/glut.h>


enum Axes{aX,aY,aZ};
enum Color{White,Yellow,Orange,Red,Green,Blue};
enum Facet{fDOWN,fUP,fLEFT,fRIGHT,fFRONT,fBACK,fNULL};

int scaleLeft=0;
int scaleTop=0;
int scaleRight=400;
int scaleBottom=400;

const double Pi = M_PI;

int ww=500; 
int hh=500;

bool filled=false;


void init()
{
	glClearColor(1.0,1.0,0.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,scaleRight,scaleBottom,0);
}

void drawSquare(
	double x1, double y1,
	double x2, double y2,
	double x3, double y3,
	double x4, double y4,
	unsigned int color,
	int id,GLenum mode
	)
{
	if(mode==GL_SELECT) glLoadName(id);
	GLfloat r = ((color>>16)%256)/255.0;
	GLfloat g = ((color>>8)%256)/255.0;
	GLfloat b = (color%256)/255.0;
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor3f(r,g,b);
	glBegin(GL_POLYGON);
		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
		glVertex2d(x3,y3);
		glVertex2d(x4,y4);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
		glVertex2d(x3,y3);
		glVertex2d(x4,y4);
	glEnd();
}

Color initialColorMap[6][3][3];

template<typename T>
std::vector<T> vec(T a1,T a2)
{
	T data[]={a1,a2};
	std::vector<T> v(data,data+2);
	return v;
}

template<typename T>
std::vector<T> vec(T a1,T a2,T a3)
{
	T data[]={a1,a2,a3};
	std::vector<T> v(data,data+3);
	return v;
}

template<typename T>
std::vector<T> vec(T a1,T a2,T a3,T a4)
{
	T data[]={a1,a2,a3,a4};
	std::vector<T> v(data,data+4);
	return v;
}

template<typename T>
std::vector<T> vec(T a1,T a2,T a3,T a4,T a5,T a6)
{
	T data[]={a1,a2,a3,a4,a5,a6};
	std::vector<T> v(data,data+6);
	return v;
}

template<typename T>
std::vector<T> vec(T a1,T a2,T a3,T a4,T a5,T a6,T a7,T a8)
{
	T data[]={a1,a2,a3,a4,a5,a6,a7,a8};
	std::vector<T> v(data,data+8);
	return v;
}

template<typename T>
std::vector<T> vec(T a1,T a2,T a3,T a4,T a5,T a6,T a7,T a8,T a9,T a10,T a11,T a12)
{
	T data[]={a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12};
	std::vector<T> v(data,data+12);
	return v;
}

int getColor(Color color)
{
	switch(color)
	{
	case White: 	return 0xffffff;
	case Yellow: 	return 0xffff00;
	case Orange: 	return 0xff7700;
	case Red: 		return 0xff0000;
	case Green: 	return 0x00ff00;
	case Blue: 		return 0x0000ff;
	}
}

class DoublePoint
{
public:
	DoublePoint(){}
	DoublePoint(double x,double y)
	:_x(x),_y(y),_z(0){}
	DoublePoint(double x,double y,double z)
	:_x(x),_y(y),_z(z){}
	
	double _x;
	double _y;
	double _z;
};

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

DoublePoint rotate2D(DoublePoint p,Axes axes,double theta)
{
	double x=p._x,y=p._y,z=p._z;
	double st=std::sin(theta),ct=std::cos(theta);
	switch(axes)
	{
	case aX:
		return DoublePoint(x,y*ct-z*st,y*st+z*ct);
	case aY:
		return DoublePoint(z*st+x*ct,y,z*ct-x*st);
	default:
		return DoublePoint(x*ct-y*st,x*st+y*ct,z);
	}
}

DoublePoint rotate3D(DoublePoint p,double theta,double phi)
{
	double x=p._x,y=p._y,z=p._z,nx,ny,nz;
	
	double 	st=std::sin(theta),	ct=std::cos(theta),
			sp=std::sin(phi),	cp=std::cos(phi);
	nx=x*ct+y*st;
	ny=x*st-y*ct;
	x=nx;y=ny;
	nx=x*cp+z*sp;
	nz=x*sp-z*cp;
	x=nx;z=nz;
	return DoublePoint(x,y,z);
}


class Square
{
public:
	Square(const std::vector<DoublePoint>& points,Color color,Facet facet,int id)
	:_points(points),_color(color),_facet(facet),_id(id)
	{}
	Square(){}
	std::vector<DoublePoint> _points;
	Color _color;
	Facet _facet;
	int _id;
};

class PlaneSquare
{
public:
	PlaneSquare(const std::vector<DoublePoint>& points)
	:_points(points){}
	PlaneSquare(){}
	std::vector<DoublePoint> _points;
};

std::vector<Square> Squares;

DoublePoint threeDTo2D(DoublePoint p)
{
	double x=p._x,y=p._y,z=p._z;
	return DoublePoint(-x/2+y+200,x/2-z+200);
}
PlaneSquare plane(const Square &square)
{
	PlaneSquare planeSquare;
	planeSquare._points.resize(4);
	for(int i=0; i<4; i++)
		planeSquare._points[i]=threeDTo2D(square._points[i]);
	return planeSquare;
}
void drawSquare(Square square,GLenum mode)
{
	PlaneSquare planeSquare=plane(square);
	//Drawings
	drawSquare(
		planeSquare._points[0]._x,planeSquare._points[0]._y,
		planeSquare._points[1]._x,planeSquare._points[1]._y,
		planeSquare._points[2]._x,planeSquare._points[2]._y,
		planeSquare._points[3]._x,planeSquare._points[3]._y,
		getColor(square._color), square._id, mode
	);
}

void drawSquares(GLenum mode)
{
	for(int i=0; i<Squares.size(); i++)
		drawSquare(Squares[i],mode);
}

double pointDepth(DoublePoint point)
{
	return 2*point._x+point._y+point._z;
}

double pointsDepth(const Square& square)
{
	double sum=0;
	for(int i=0; i<4; i++)
		sum+=pointDepth(square._points[i]);
	return sum;
}
void drawQuar(const Square &square)
{
	Squares.push_back(square);
	int i=Squares.size()-1;
	Square tmp;
	while(true)
	{
		if(i==0) return;
		if(pointsDepth(Squares[i-1])<pointsDepth(Squares[i])) return;
		tmp=Squares[i];
		Squares[i]=Squares[i-1];
		Squares[i-1]=tmp;
		i--;
	}
}

class Cubic
{
public:
	Cubic(	double x,double y,double z,double l,double w,double h,double t,double p,double g,
			const std::vector<int> & id)
	:_x(x),_y(y),_z(z),_length(l),_width(w),_height(h),_theta(t),_phi(p),_gamma(g),
	_xd(0),_yd(0),_zd(0),_axes(aX),_rotate(0),_colors(vec(White,White,White,White,White,White)),
	_id(id)
	{}
	Cubic(){}
	void draw(GLenum mode)
	{
		Squares.clear();
		std::vector<Square> s=squares();
		for(int i=0; i<6; i++)
			drawQuar(s[i]);
		drawSquares(mode);
	}
	std::vector<Square> squares() const
	{
		double 	l=_length, w=_width, h=_height;
		double	t=_theta,  p=_phi,   g=_gamma;
		std::vector<DoublePoint> points=vec(
			DoublePoint(-l/2, w/2 , h/2 ),
			DoublePoint(-l/2, w/2 , -h/2),
			DoublePoint(-l/2, -w/2, h/2 ),
			DoublePoint(-l/2, -w/2, -h/2),
			DoublePoint(l/2 , w/2 , h/2 ),
			DoublePoint(l/2 , w/2 , -h/2),
			DoublePoint(l/2 , -w/2, h/2 ),
			DoublePoint(l/2 , -w/2, -h/2)
		);
		
		for(int i=0; i<8; i++)
		{
			points[i]._x+=_xd;
			points[i]._y+=_yd;
			points[i]._z+=_zd;
			if(_rotate!=0)
				points[i]=rotate2D(points[i],_axes,_rotate);
			points[i]=rotate3D(points[i],t,p);
			points[i]._x+=_x;
			points[i]._y+=_y;
			points[i]._z+=_z;
		}
		return vec(
				Square(vec(points[1],points[3],points[7],points[5]),
					_colors[0],fDOWN,	(_id[0]*9+_id[1]*3+_id[2])*6+0),
				Square(vec(points[0],points[2],points[6],points[4]),
					_colors[1],fUP,		(_id[0]*9+_id[1]*3+_id[2])*6+1),
				Square(vec(points[0],points[1],points[5],points[4]),
					_colors[2],fLEFT,	(_id[0]*9+_id[1]*3+_id[2])*6+2),
				Square(vec(points[2],points[3],points[7],points[6]),
					_colors[3],fRIGHT,	(_id[0]*9+_id[1]*3+_id[2])*6+3),
				Square(vec(points[4],points[5],points[7],points[6]),
					_colors[4],fFRONT,	(_id[0]*9+_id[1]*3+_id[2])*6+4),
				Square(vec(points[0],points[1],points[3],points[2]),
					_colors[5],fBACK,	(_id[0]*9+_id[1]*3+_id[2])*6+5)
			   );
	}
	DoublePoint position() const
	{
		DoublePoint p(_xd,_yd,_zd);
		if(_rotate!=0)
			p=rotate2D(p,_axes,_rotate);
		p=rotate3D(p,_theta,_phi);
		p._x+=_x;
		p._y+=_y;
		p._z+=_z;
		return p;
	}
	void displace(double x,double y,double z)
	{
		_xd=x;
		_yd=y;
		_zd=z;
	}
	void rotateAlongAxes(Axes axes,double rotation)
	{
		_axes=axes;
		_rotate=rotation;
	}
	std::vector<Color> _colors;
	std::vector<int> _id;
private:
	double _x;
	double _y;
	double _z;
	double _length;
	double _width;
	double _height;
	double _theta;
	double _phi;
	double _gamma;
	double _xd;
	double _yd;
	double _zd;
	Axes _axes;
	double _rotate;
};
std::vector<Cubic> Cubics;

void drawCubic(Cubic cbc)
{
	Cubics.push_back(cbc);
	
	int i=Cubics.size()-1;
	Cubic tmp;
	while(true)
	{
		if(i==0) return;
		if(pointDepth(Cubics[i-1].position())
			<pointDepth(Cubics[i].position())) return;
		tmp=Cubics[i];
		Cubics[i]=Cubics[i-1];
		Cubics[i-1]=tmp;
		i--;
	}
}
void drawCubics(GLenum mode)
{
	for(int i=0; i<Cubics.size(); i++)
	{
		Cubics[i].draw(mode);
	}
}

class MagicCube
{
public:
	MagicCube(Color colorMap[][3][3])
	:_direction(vec(0.0,0.0)),_axes(aX),
	_rotation(vec(0.0,0.0,0.0))
	{
		for(int i=0; i<6; i++)
			for(int j=0; j<6; j++)
				for(int k=0; k<6; k++)
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
					array2Step(_colors,vec(vec(5,5,5,5,5,5,5,5),
										vec(1,2,2,2,1,0,0,0),
										vec(2,2,1,0,0,0,1,2)));
					array3Step(_colors,vec(vec(2,2,2,0,0,0,3,3,3,1,1,1),
										vec(0,0,0,0,0,0,0,0,0,0,0,0),
										vec(2,1,0,2,1,0,0,1,2,0,1,2)));
				}
				else
				{
					array2Step(_colors,vec(vec(5,5,5,5,5,5,5,5),
										vec(0,0,0,1,2,2,2,1),
										vec(2,1,0,0,0,1,2,2)));
					array3Step(_colors,vec(vec(1,1,1,3,3,3,0,0,0,2,2,2),
										vec(0,0,0,0,0,0,0,0,0,0,0,0),
										vec(2,1,0,2,1,0,0,1,2,0,1,2)));
				}
				break;
			case 1:
				if(direction)
				{
					array3Step(_colors,vec(vec(2,2,2,0,0,0,3,3,3,1,1,1),
										vec(1,1,1,1,1,1,1,1,1,1,1,1),
										vec(2,1,0,2,1,0,0,1,2,0,1,2)));
				}                       
				else                    
				{                       
					array3Step(_colors,vec(vec(1,1,1,3,3,3,0,0,0,2,2,2),
										vec(1,1,1,1,1,1,1,1,1,1,1,1),
										vec(2,1,0,2,1,0,0,1,2,0,1,2)));
				}                       
				break;                  
			case 2:                     
				if(direction)
				{
					array2Step(_colors,vec(vec(4,4,4,4,4,4,4,4),
										vec(1,2,2,2,1,0,0,0),
										vec(2,2,1,0,0,0,1,2)));
					array3Step(_colors,vec(vec(2,2,2,0,0,0,3,3,3,1,1,1),
										vec(2,2,2,2,2,2,2,2,2,2,2,2),
										vec(2,1,0,2,1,0,0,1,2,0,1,2)));
				}
				else
				{
					array2Step(_colors,vec(vec(4,4,4,4,4,4,4,4),
										vec(0,0,0,1,2,2,2,1),
										vec(2,1,0,0,0,1,2,2)));
					array3Step(_colors,vec(vec(1,1,1,3,3,3,0,0,0,2,2,2),
										vec(2,2,2,2,2,2,2,2,2,2,2,2),
										vec(2,1,0,2,1,0,0,1,2,0,1,2)));
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
					array2Step(_colors,vec(vec(3,3,3,3,3,3,3,3),
										vec(0,0,0,1,2,2,2,1),
										vec(2,1,0,0,0,1,2,2)));
					array3Step(_colors,vec(vec(1,1,1,5,5,5,0,0,0,4,4,4),
										vec(2,1,0,0,0,0,0,1,2,0,0,0),
										vec(0,0,0,2,1,0,0,0,0,0,1,2)));
				}
				else
				{
					array2Step(_colors,vec(vec(3,3,3,3,3,3,3,3),
										vec(1,2,2,2,1,0,0,0),
										vec(2,2,1,0,0,0,1,2)));
					array3Step(_colors,vec(vec(4,4,4,0,0,0,5,5,5,1,1,1),
										vec(0,0,0,2,1,0,0,0,0,0,1,2),
										vec(2,1,0,0,0,0,0,1,2,0,0,0)));
				}
				break;
			case 1:
				if(direction)
				{
					array3Step(_colors,vec(vec(1,1,1,5,5,5,0,0,0,4,4,4),
										vec(2,1,0,1,1,1,0,1,2,1,1,1),
										vec(1,1,1,2,1,0,1,1,1,0,1,2)));
				}
				else
				{
					array3Step(_colors,vec(vec(4,4,4,0,0,0,5,5,5,1,1,1),
										vec(1,1,1,2,1,0,1,1,1,0,1,2),
										vec(2,1,0,1,1,1,0,1,2,1,1,1)));
				}
				break;
			case 2:
				if(direction)
				{
					array2Step(_colors,vec(vec(2,2,2,2,2,2,2,2),
										vec(0,0,0,1,2,2,2,1),
										vec(2,1,0,0,0,1,2,2)));
					array3Step(_colors,vec(vec(1,1,1,5,5,5,0,0,0,4,4,4),
										vec(2,1,0,2,2,2,0,1,2,2,2,2),
										vec(2,2,2,2,1,0,2,2,2,0,1,2)));
				}
				else
				{
					array2Step(_colors,vec(vec(2,2,2,2,2,2,2,2),
										vec(1,2,2,2,1,0,0,0),
										vec(2,2,1,0,0,0,1,2)));
					array3Step(_colors,vec(vec(4,4,4,0,0,0,5,5,5,1,1,1),
										vec(2,2,2,2,1,0,2,2,2,0,1,2),
										vec(2,1,0,2,2,2,0,1,2,2,2,2)));
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
		double eps=0.15;
		return (std::fabs(_rotation[0])<eps && 
				std::fabs(_rotation[1])<eps &&
				std::fabs(_rotation[2])<eps);
	}
	int rotate(Axes axes,int layer,double rotation)
	{
		double eps=0.05;
		if(axes!=_axes && !noRotation())
			return -1;
			
		if(axes!=_axes)
			_rotation=vec(0.0,0.0,0.0);
		
		_rotation[layer]+=rotation;
		_axes=axes;
		
		if(_rotation[layer]<-Pi/2)
		{
			_rotation[layer]=0;
			rotateColor(axes,layer,false);
			return 1;
		}
		else if(_rotation[layer]>=Pi/2)
		{
			_rotation[layer]=0;
			rotateColor(axes,layer,true);
			return 1;
		}
		
		//if(std::fabs(_rotation[layer])<eps)
		//	_rotation[layer]=0;
		
		return 0;
	}
	void rotate3D(double t,double p)
	{
		_direction[0] += t;
		_direction[1] += p;
		while(_direction[0]>=2*Pi)
			_direction[0]-=2*Pi;
		while(_direction[0]<0)
			_direction[0]+=2*Pi;
		while(_direction[1]>=2*Pi)
			_direction[1]-=2*Pi;
		while(_direction[1]<0)
			_direction[1]+=2*Pi;
	}
	std::vector<std::vector<std::vector<Cubic> > >cubics()
	{
		double t=_direction[0],p=_direction[1],g=_direction[2];
		std::vector<std::vector<std::vector<Cubic> > > cubics;
		cubics.resize(3);
		for(int i=0; i<3; i++)
		{
			cubics[i].resize(3);
			for(int j=0; j<3; j++)
			{
				cubics[i][j].resize(3);
				for(int k=0; k<3; k++)
				{
					cubics[i][j][k]=Cubic(0,0,0,50,50,50,t,p,g,vec(i,j,k));
					cubics[i][j][k].displace((i-1)*50,(j-1)*50,(k-1)*50);
					switch(_axes)
					{
					case aX:
						cubics[i][j][k].rotateAlongAxes(_axes,_rotation[i]);
						break;
					case aY:
						cubics[i][j][k].rotateAlongAxes(_axes,_rotation[j]);
						break;
					case aZ:
						cubics[i][j][k].rotateAlongAxes(_axes,_rotation[k]);
						break;
					}
				}
			}
		}
		for(int i=0; i<3; i++)
			for(int j=0; j<3; j++)
			{
				cubics[i][j][0]._colors[0]=_colors[0][i][j];
				cubics[i][j][2]._colors[1]=_colors[1][i][j];
				cubics[i][2][j]._colors[2]=_colors[2][i][j];
				cubics[i][0][j]._colors[3]=_colors[3][i][j];
				cubics[2][i][j]._colors[4]=_colors[4][i][j];
				cubics[0][i][j]._colors[5]=_colors[5][i][j];
			}
		return cubics;
	}
	void draw(GLenum mode)
	{
		std::vector<std::vector<std::vector<Cubic> > > cs=cubics();
		switch(_axes)
		{
		case aX:
			if(pointDepth(cs[0][1][1].position())<pointDepth(cs[2][1][1].position()))
				for(int i=0; i<3; i++)
				{
					Cubics.clear();
					for(int j=0; j<3; j++)
						for(int k=0; k<3; k++)
							drawCubic(cs[i][j][k]);
					drawCubics(mode);
				}
			else
				for(int i=2; i>=0; i--)
				{
					Cubics.clear();
					for(int j=0; j<3; j++)
						for(int k=0; k<3; k++)
							drawCubic(cs[i][j][k]);
					drawCubics(mode);
				}
			break;
		case aY:
			if(pointDepth(cs[1][0][1].position())<pointDepth(cs[1][2][1].position()))
				for(int i=0; i<3; i++)
				{
					Cubics.clear();
					for(int j=0; j<3; j++)
						for(int k=0; k<3; k++)
							drawCubic(cs[k][i][j]);
					drawCubics(mode);
				}
			else
				for(int i=2; i>=0; i--)
				{
					Cubics.clear();
					for(int j=0; j<3; j++)
						for(int k=0; k<3; k++)
							drawCubic(cs[k][i][j]);
					drawCubics(mode);
				}
			break;
		case aZ:
			if(pointDepth(cs[1][1][0].position())<pointDepth(cs[1][1][2].position()))
				for(int i=0; i<3; i++)
				{
					Cubics.clear();
					for(int j=0; j<3; j++)
						for(int k=0; k<3; k++)
							drawCubic(cs[j][k][i]);
					drawCubics(mode);
				}
			else
				for(int i=2; i>=0; i--)
				{
					Cubics.clear();
					for(int j=0; j<3; j++)
						for(int k=0; k<3; k++)
							drawCubic(cs[j][k][i]);
					drawCubics(mode);
				}
			break;
		}
	}
	
	std::vector<double> _direction;
	std::vector<double> _rotation;
	Axes _axes;
private:
	Color _colors[6][3][3];
};

MagicCube magicCube(initialColorMap);



void initialize()
{
	Squares.clear();
	Cubics.clear();
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

const int sbSize = 4096;

int selected = -1;
int mousex, mousey;

void handleMouseEvent(int button, int st, int xx, int yy)
{
	int x,y;
	if(ww<hh)
	{
		x = (double)xx / ww * (scaleRight - scaleLeft) + scaleLeft;
		y = (double)yy / ww * (scaleBottom - scaleTop) + scaleTop;
	}
	else
	{
		x = (double)xx / hh * (scaleRight - scaleLeft) + scaleLeft;
		y = (double)yy / hh * (scaleBottom - scaleTop) + scaleTop;
	}
	if(st==GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			GLuint selectBuf[sbSize];
			GLint hits;
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT,viewport);
			glSelectBuffer(sbSize,selectBuf);
			glRenderMode(GL_SELECT);
			glInitNames();
			glPushName(0);
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			gluPickMatrix ((GLdouble) xx, (GLdouble)(viewport[3]- yy), 
				0.01, 0.01, viewport);
			
			if(ww<hh)
				gluOrtho2D(0,400,400*(GLfloat)hh/(GLfloat)ww,0);
			else
				gluOrtho2D(0,400*(GLfloat)ww/(GLfloat)hh,400,0);
				
			magicCube.draw(GL_SELECT);
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glFlush();
			hits=glRenderMode(GL_RENDER);
			if(hits>0)
			{
				GLint names, *ptr;
				ptr = (GLint *) selectBuf; 
				for (int i = 0; i < hits; i++)
				{	/*  for each hit  */
					names = *ptr;
					ptr+=3;
					for (int j = 0; j < names; j++)
					{ /*  for each name */
						selected = *ptr;
						ptr++;
					}
				}
			}
			else
				selected = -1;
		}
		else
			selected = -1;
	}
	else
		selected = -1;
	mousex = x;
	mousey = y;
}
void handleMouseMoveEvent(int xx, int yy)
{
	int x,y;
	if(ww<hh)
	{
		x = (double)xx / ww * (scaleRight - scaleLeft) + scaleLeft;
		y = (double)yy / ww * (scaleBottom - scaleTop) + scaleTop;
	}
	else
	{
		x = (double)xx / hh * (scaleRight - scaleLeft) + scaleLeft;
		y = (double)yy / hh * (scaleBottom - scaleTop) + scaleTop;
	}
	if(selected < 0) return;
	int sx, sy, sz, f;
	f = selected % 6;
	sz = (selected / 6) % 3;
	sy = (selected / 18) % 3;
	sx = (selected / 54) % 3;
	int sum = 0;
	if(sx==1) sum++;
	if(sy==1) sum++;
	if(sz==1) sum++;
	
	if(	sum==2 )
	{
		double dx=x-mousex;
		double dy=y-mousey;
		if(magicCube._direction[1]<Pi/2 || magicCube._direction[1]>3*Pi/2)
			magicCube.rotate3D(dx/50,-dy/50);
		else
			magicCube.rotate3D(-dx/50,-dy/50);
	}
	else if(sum==1)
	{
		double dx=x-mousex;
		double dy=y-mousey;
		std::vector<int> targetCubic = vec(sx,sy,sz);
		Axes axes;
		int index;
		if(	(targetCubic==vec(0,0,1) && (Facet)f==fRIGHT) ||
			(targetCubic==vec(0,1,0) && (Facet)f==fDOWN ) ||
			(targetCubic==vec(0,2,1) && (Facet)f==fLEFT ) ||
			(targetCubic==vec(0,1,2) && (Facet)f==fUP   ) )
		{
			if(magicCube._axes==aX || magicCube.noRotation())
			{
				axes=aX;
				index=0;
			}
			else
			{
				selected = -1;
				return;
			}
		}
		else if((targetCubic==vec(2,0,1) && (Facet)f==fRIGHT) ||
				(targetCubic==vec(2,1,0) && (Facet)f==fDOWN ) ||
				(targetCubic==vec(2,2,1) && (Facet)f==fLEFT ) ||
				(targetCubic==vec(2,1,2) && (Facet)f==fUP   ))
		{
			if(magicCube._axes==aX || magicCube.noRotation())
			{
				axes=aX;
				index=2;
			}
			else
			{
				selected = -1;
				return;
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
			else
			{
				selected = -1;
				return;
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
			else
			{
				selected = -1;
				return;
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
			else
			{
				selected = -1;
				return;
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
			else
			{
				selected = -1;
				return;
			}
		}
		else
		{
			selected = -7;
			return;
		}
		
		switch(axes)
		{
		case aX:
			if(f==(int)fRIGHT || f==(int)fLEFT)
				if(	(magicCube._direction[1]<Pi/2 || magicCube._direction[1]>3*Pi/2))
					if((magicCube._direction[0]<0.44 || magicCube._direction[0]>0.44+Pi))
						magicCube.rotate(axes,index,-dy/50);
					else
						magicCube.rotate(axes,index,dy/50);
				else
					if((magicCube._direction[0]<5.86-Pi || magicCube._direction[0]>5.86))
						magicCube.rotate(axes,index,dy/50);
					else
						magicCube.rotate(axes,index,-dy/50);
			else if(f==(int)fDOWN || f==(int)fUP)
				if(	(magicCube._direction[1]<Pi/2 || magicCube._direction[1]>3*Pi/2))
					if((magicCube._direction[0]<2.00 || magicCube._direction[0]>2.00+Pi))
						magicCube.rotate(axes,index,-dx/50);
					else
						magicCube.rotate(axes,index,dx/50);
				else
					if((magicCube._direction[0]<4.3-Pi || magicCube._direction[0]>4.3))
						magicCube.rotate(axes,index,dx/50);
					else
						magicCube.rotate(axes,index,-dx/50);
			else
				magicCube.rotate(axes,index,dy/50);
			break;
		case aY:
			if(f==(int)fFRONT || f==(int)fBACK)
				if(	(magicCube._direction[1]<Pi/2 || magicCube._direction[1]>3*Pi/2))
					if((magicCube._direction[0]<2.00 || magicCube._direction[0]>2.00+Pi))
						magicCube.rotate(axes,index,-dy/50);
					else
						magicCube.rotate(axes,index,dy/50);
				else
					if((magicCube._direction[0]<4.3-Pi || magicCube._direction[0]>4.3))
						magicCube.rotate(axes,index,-dy/50);
					else
						magicCube.rotate(axes,index,dy/50);
			else if(f==(int)fDOWN || f==(int)fUP)
				if(	(magicCube._direction[1]<Pi/2 || magicCube._direction[1]>3*Pi/2))
					if((magicCube._direction[0]<0.44 || magicCube._direction[0]>0.44+Pi))
						magicCube.rotate(axes,index,dx/50);
					else
						magicCube.rotate(axes,index,-dx/50);
				else
					if((magicCube._direction[0]<5.86-Pi || magicCube._direction[0]>5.86))
						magicCube.rotate(axes,index,dx/50);
					else
						magicCube.rotate(axes,index,-dx/50);
			else
				magicCube.rotate(axes,index,dy/50);
			break;
		case aZ:
			if((magicCube._direction[1]<Pi/2 || magicCube._direction[1]>3*Pi/2))
				magicCube.rotate(axes,index,-dx/50);
			else
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


bool endfunc()
{
	return !magicCube.noRotation();
}

void fp()
{
	magicCube.rotate(aX,2,0.2);
}
void bp()
{
	magicCube.rotate(aX,0,-0.2);
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
	magicCube.rotate(aZ,0,-0.2);
}
void up()
{
	magicCube.rotate(aZ,2,0.2);
}void fn()
{
	magicCube.rotate(aX,2,-0.2);
}
void bn()
{
	magicCube.rotate(aX,0,0.2);
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
	magicCube.rotate(aZ,0,0.2);
}
void un()
{
	magicCube.rotate(aZ,2,-0.2);
}


bool positive=true;
void handleKeyEvent(unsigned char keyCode, int x, int y)
{
	if(keyCode=='c')
		initialize();
	if(keyCode=='p')
		positive=true;
	if(keyCode=='n')
		positive=false;
	if(keyCode=='d')
		if(glutGetModifiers()==GLUT_ACTIVE_ALT)
			dn();
		else
			dp();
	if(keyCode=='u')
		if(glutGetModifiers()==GLUT_ACTIVE_ALT)
			un();
		else
			up();
	if(keyCode=='l')
		if(glutGetModifiers()==GLUT_ACTIVE_ALT)
			ln();
		else
			lp();
	if(keyCode=='r')
		if(glutGetModifiers()==GLUT_ACTIVE_ALT)
			rn();
		else
			rp();
	if(keyCode=='f')
		if(glutGetModifiers()==GLUT_ACTIVE_ALT)
			fn();
		else
			fp();
	if(keyCode=='b')
		if(glutGetModifiers()==GLUT_ACTIVE_ALT)
			bn();
		else
			bp();
	if(keyCode=='q' && glutGetModifiers()==GLUT_ACTIVE_ALT)
		exit(0);
	glutPostRedisplay();
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	magicCube.draw(GL_RENDER);
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w<h)
		gluOrtho2D(0,400,400*(GLfloat)h/(GLfloat)w,0);
	else
		gluOrtho2D(0,400*(GLfloat)w/(GLfloat)h,400,0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,w,h);
	ww = w;
	hh = h;
}

void idle()
{
	//dn();
	//glutPostRedisplay();
}


#endif
