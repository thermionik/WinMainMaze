//g_math.cpp

#include "g_util.h"

GLvoid g_util::trigInit( GLvoid )
{
	GLuint loop;				//looping index, for the angle
	GLfloat radian;

	for(loop=0; loop<720; loop++)	//Loop through the 720 angles
	{
		//convert angle to radians
		radian=(float)DEG_TO_RAD(loop);	//Turn degrees to radians

		//fill in the tables
		SIN[loop]=(float)sin(radian);	//Fill in the tables
		COS[loop]=(float)cos(radian);
	}
}


int pickaRandom( int min, int max ) 
{	
	return (rand()%(max-min+1))+min;	
}

//------------------------------------------------------------------//
//- void ComputeNormal(VECTOR*, VECTOR*, VECTOR*) ------------------//
//------------------------------------------------------------------//
//- Description: Sends a surface normal (composed of 3 vectors), to-//
//-              OpenGL.										   -//
//------------------------------------------------------------------//
void ComputeNormal(VECTOR* v1, VECTOR* v2, VECTOR* v3)
{
	VECTOR a(0.0f, 0.0f, 0.0f);
	VECTOR b(0.0f, 0.0f, 0.0f);
	VECTOR result(0.0f, 0.0f, 0.0f);
	float length;

	a.vector[0]= v1->vector[0] - v2->vector[0];
	a.vector[1]= v1->vector[1] - v2->vector[1];
	a.vector[2]= v1->vector[2] - v2->vector[2];

	b.vector[0]= v1->vector[0] - v3->vector[0];
	b.vector[1]= v1->vector[1] - v3->vector[1];
	b.vector[2]= v1->vector[2] - v3->vector[2];

	result.vector[0]= (a.vector[1]*b.vector[2]) - (b.vector[1]*a.vector[2]);
	result.vector[1]= (b.vector[0]*a.vector[2]) - (a.vector[0]*b.vector[2]);
	result.vector[2]= (a.vector[0]*b.vector[1]) - (b.vector[0]*a.vector[1]);

	// calculate the length of the normal
	length= (float)sqrtf(SQUARE(result.vector[0]) + 
						 SQUARE(result.vector[1]) + 
						 SQUARE(result.vector[2]));

	// normalize and specify the normal
	glNormal3f(result.vector[0]/length, result.vector[1]/length, result.vector[2]/length);
}

float VECTOR::ComputeLength(void)
{
	float length;

	// calculate the length of the normal
	length= (float)sqrt(SQUARE(vector[0]) + 
						SQUARE(vector[1]) + 
						SQUARE(vector[2]));

	return length;
}

float ComputeDotProduct(VECTOR* v1, VECTOR* v2)
{
	float dot;

	dot= (v1->vector[0]+v2->vector[0])+
		 (v1->vector[1]+v2->vector[1])+
		 (v1->vector[2]+v2->vector[2]);

	return dot;
}

VECTOR ComputeCrossProduct(const VECTOR &v1, const VECTOR &v2)
{
	VECTOR result(0.0f, 0.0f, 0.0f);

	result.vector[0]= (v1.vector[1]*v2.vector[2]) - (v1.vector[2]*v2.vector[1]);
	result.vector[1]= (v1.vector[2]*v2.vector[0]) - (v1.vector[0]*v2.vector[2]);
	result.vector[2]= (v1.vector[0]*v2.vector[1]) - (v1.vector[1]*v2.vector[0]);

	return result;
}

VECTOR::VECTOR(float x, float y, float z)
{
	vector[0]=x;
	vector[1]=y;
	vector[2]=z;
}

inline VECTOR VECTOR::operator+ (const VECTOR &v)
{
	VECTOR result(0.0f, 0.0f, 0.0f);

	result.vector[0]= vector[0] + v.vector[0];
	result.vector[1]= vector[1] + v.vector[1];
	result.vector[2]= vector[2] + v.vector[2];

	return result;
}

inline VECTOR VECTOR::operator- (const VECTOR &v)
{
	VECTOR result(0.0f, 0.0f, 0.0f);

	result.vector[0]= vector[0] - v.vector[0];
	result.vector[1]= vector[1] - v.vector[1];
	result.vector[2]= vector[2] - v.vector[2];

	return result;
}

inline VECTOR VECTOR::operator* (const float scalar)
{
	VECTOR result(0.0f, 0.0f, 0.0f);

	result.vector[0]= vector[0] * scalar;
	result.vector[1]= vector[1] * scalar;
	result.vector[2]= vector[2] * scalar;

	return result;
}

inline VECTOR VECTOR::operator* (const VECTOR &v)
{
	VECTOR result(0.0f, 0.0f, 0.0f);

	result.vector[0]= vector[0] * v.vector[0];
	result.vector[1]= vector[1] * v.vector[1];
	result.vector[2]= vector[2] * v.vector[2];

	return result;
}

inline VECTOR VECTOR::operator/ (const VECTOR &v)
{
	VECTOR result(0.0f, 0.0f, 0.0f);

	result.vector[0]= vector[0] / v.vector[0];
	result.vector[1]= vector[1] / v.vector[1];
	result.vector[2]= vector[2] / v.vector[2];

	return result;
}

VERTEX::VERTEX(float x, float y, float z)
{
	vertex[0]=x;
	vertex[1]=y;
	vertex[2]=z;
}

void VERTEX::SendToOGL(void)
{	
	glVertex3fv(vertex);	
}

//------------------------------------------------------------------//
//- inline VERTEX VERTEX::operator+ (VERTEX) -----------------------//
//------------------------------------------------------------------//
//- Description: Overloading the + operator, allowing you to add   -//
//-				 one vertex to another, with some nice looking     -//
//-				 code.											   -//
//------------------------------------------------------------------//
inline VERTEX VERTEX::operator+ (const VERTEX &v)
{
	VERTEX result(0.0f, 0.0f, 0.0f);

	result.vertex[0]= vertex[0] + v.vertex[0];
	result.vertex[1]= vertex[1] + v.vertex[1];
	result.vertex[2]= vertex[2] + v.vertex[2];

	return result;
}

//------------------------------------------------------------------//
//- inline VERTEX VERTEX::operator- (VERTEX) -----------------------//
//------------------------------------------------------------------//
//- Description: Overloading the - operator, allowing you to	   -//
//-				 subtract one vertex to another, with some nice	   -//
//-				 looking code.									   -//
//------------------------------------------------------------------//
inline VERTEX VERTEX::operator- (const VERTEX &v)
{
	VERTEX result(0.0f, 0.0f, 0.0f);

	result.vertex[0]= vertex[0] - v.vertex[0];
	result.vertex[1]= vertex[1] - v.vertex[1];
	result.vertex[2]= vertex[2] - v.vertex[2];

	return result;
}


//------------------------------------------------------------------//
//- inline VECTOR VECTOR::operator* (float) ------------------------//
//------------------------------------------------------------------//
//- Description: Overloading the * operator, this allows you to    -//
//-				 multiply a every component of a vertex by a scalar-//
//-				 (single value).								   -//
//------------------------------------------------------------------//
inline VERTEX VERTEX::operator* (const float scalar)
{
	VERTEX result(0.0f, 0.0f, 0.0f);

	result.vertex[0]= vertex[0] * scalar;
	result.vertex[1]= vertex[1] * scalar;
	result.vertex[2]= vertex[2] * scalar;

	return result;
}

//------------------------------------------------------------------//
//- inline VERTEX VERTEX::operator* (VERTEX) -----------------------//
//------------------------------------------------------------------//
//- Description: Overloading the * operator.  But this time it is  -//
//-				 for vertex by vertex multiplication.			   -//
//------------------------------------------------------------------//
inline VERTEX VERTEX::operator* (const VERTEX &v)
{
	VERTEX result(0.0f, 0.0f, 0.0f);

	result.vertex[0]= vertex[0] * v.vertex[0];
	result.vertex[1]= vertex[1] * v.vertex[1];
	result.vertex[2]= vertex[2] * v.vertex[2];

	return result;
}

//------------------------------------------------------------------//
//- inline VERTEX VERTEX::operator/ (VERTEX) -----------------------//
//------------------------------------------------------------------//
//- Description: Overloading the / operator, allowing you to divide-//
//-				 one vertex to another.							   -//
//------------------------------------------------------------------//
inline VERTEX VERTEX::operator/ (const VERTEX &v)
{
	VERTEX result(0.0f, 0.0f, 0.0f);

	result.vertex[0]= vertex[0] / v.vertex[0];
	result.vertex[1]= vertex[1] / v.vertex[1];
	result.vertex[2]= vertex[2] / v.vertex[2];

	return result;
}


//------------------------------------------------------------------//
//- void MATRIX4X4::LoadIdentity(void) -----------------------------//
//------------------------------------------------------------------//
//- Description: Sets the current matrix to the zero matrix.	   -//
//-				 Zero matrix: | 0  0  0  0 |					   -//
//-							  | 0  0  0  0 |					   -//
//-							  | 0  0  0  0 |					   -//
//-							  | 0  0  0  0 |					   -//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-             matrix.ZeroIdentity();							   -//
//------------------------------------------------------------------//
void MATRIX4X4::LoadZero(void)
{
	int loop;

	for(loop=0; loop<16; loop++)
		matrix[loop]=0.0f;
}

//------------------------------------------------------------------//
//- void MATRIX4X4::LoadIdentity(void) -----------------------------//
//------------------------------------------------------------------//
//- Description: Sets the current matrix to the identity matrix.   -//
//-				 Identity matrix: | 1  0  0  0 |				   -//
//-								  | 0  1  0  0 |				   -//
//-								  | 0  0  1  0 |				   -//
//-								  | 0  0  0  1 |				   -//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-             matrix.LoadIdentity();							   -//
//------------------------------------------------------------------//
void MATRIX4X4::LoadIdentity(void)
{
	matrix[0] =1.0f; matrix[1] =0.0f; matrix[2] =0.0f; matrix[3] =0.0f;
	matrix[4] =0.0f; matrix[5] =1.0f; matrix[6] =0.0f; matrix[7] =0.0f;
	matrix[8] =0.0f; matrix[9] =0.0f; matrix[10]=1.0f; matrix[11]=0.0f;
	matrix[12]=0.0f; matrix[13]=0.0f; matrix[14]=0.0f; matrix[15]=1.0f;
}

//------------------------------------------------------------------//
//- MATRIX4X4 operator+ (MATRIX4X4, MATRIX4X4) ---------------------//
//------------------------------------------------------------------//
//- Description: Overloading the + operator.  This adds the        -//
//-				 contents of one matrix to another, and returns the-//
//-				 results.										   -//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-					result= matrix1+matrix2;					   -//
//------------------------------------------------------------------//
MATRIX4X4 operator+ (const MATRIX4X4 &m1, const MATRIX4X4 &m2)
{
	MATRIX4X4 result;

	result.matrix[0]= m1.matrix[0]+m2.matrix[0];
	result.matrix[1]= m1.matrix[1]+m2.matrix[1];
	result.matrix[2]= m1.matrix[2]+m2.matrix[2];
	result.matrix[3]= m1.matrix[3]+m2.matrix[3];
	result.matrix[4]= m1.matrix[4]+m2.matrix[4];
	result.matrix[5]= m1.matrix[5]+m2.matrix[5];
	result.matrix[6]= m1.matrix[6]+m2.matrix[6];
	result.matrix[7]= m1.matrix[7]+m2.matrix[7];
	result.matrix[8]= m1.matrix[8]+m2.matrix[8];
	result.matrix[9]= m1.matrix[9]+m2.matrix[9];
	result.matrix[10]=m1.matrix[10]+m2.matrix[10];
	result.matrix[11]=m1.matrix[11]+m2.matrix[11];
	result.matrix[12]=m1.matrix[12]+m2.matrix[12];
	result.matrix[13]=m1.matrix[13]+m2.matrix[13];
	result.matrix[14]=m1.matrix[14]+m2.matrix[14];
	result.matrix[15]=m1.matrix[15]+m2.matrix[15];

	return result;
}

//------------------------------------------------------------------//
//- MATRIX4X4 operator- (MATRIX4X4, MATRIX4X4) ---------------------//
//------------------------------------------------------------------//
//- Description: Overloading the - operator.  This subtracts the   -//
//-				 contents of one matrix to another, and returns the-//
//-				 results.										   -//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-					result= matrix1-matrix2;					   -//
//------------------------------------------------------------------//
MATRIX4X4 operator- (const MATRIX4X4 &m1, const MATRIX4X4 &m2)
{
	MATRIX4X4 result;

	result.matrix[0]= m1.matrix[0]-m2.matrix[0];
	result.matrix[1]= m1.matrix[1]-m2.matrix[1];
	result.matrix[2]= m1.matrix[2]-m2.matrix[2];
	result.matrix[3]= m1.matrix[3]-m2.matrix[3];
	result.matrix[4]= m1.matrix[4]-m2.matrix[4];
	result.matrix[5]= m1.matrix[5]-m2.matrix[5];
	result.matrix[6]= m1.matrix[6]-m2.matrix[6];
	result.matrix[7]= m1.matrix[7]-m2.matrix[7];
	result.matrix[8]= m1.matrix[8]-m2.matrix[8];
	result.matrix[9]= m1.matrix[9]-m2.matrix[9];
	result.matrix[10]=m1.matrix[10]-m2.matrix[10];
	result.matrix[11]=m1.matrix[11]-m2.matrix[11];
	result.matrix[12]=m1.matrix[12]-m2.matrix[12];
	result.matrix[13]=m1.matrix[13]-m2.matrix[13];
	result.matrix[14]=m1.matrix[14]-m2.matrix[14];
	result.matrix[15]=m1.matrix[15]-m2.matrix[15];

	return result;
}

//------------------------------------------------------------------//
//- MATRIX4X4 operator* (MATRIX4X4, float) -------------------------//
//------------------------------------------------------------------//
//- Description: Overloading the * operator.  This is for when you -//
//-				 want to multiply every component of a matrix by a -//
//-				 a scalar.										   -//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-					result= matrix1*2;							   -//
//------------------------------------------------------------------//
MATRIX4X4 operator* (const MATRIX4X4 &m1, const float scalar)
{
	MATRIX4X4 result;

	result.matrix[0]= m1.matrix[0]*scalar;
	result.matrix[1]= m1.matrix[1]*scalar;
	result.matrix[2]= m1.matrix[2]*scalar;
	result.matrix[3]= m1.matrix[3]*scalar;
	result.matrix[4]= m1.matrix[4]*scalar;
	result.matrix[5]= m1.matrix[5]*scalar;
	result.matrix[6]= m1.matrix[6]*scalar;
	result.matrix[7]= m1.matrix[7]*scalar;
	result.matrix[8]= m1.matrix[8]*scalar;
	result.matrix[9]= m1.matrix[9]*scalar;
	result.matrix[10]=m1.matrix[10]*scalar;
	result.matrix[11]=m1.matrix[11]*scalar;
	result.matrix[12]=m1.matrix[12]*scalar;
	result.matrix[13]=m1.matrix[13]*scalar;
	result.matrix[14]=m1.matrix[14]*scalar;
	result.matrix[15]=m1.matrix[15]*scalar;

	return result;
}

//------------------------------------------------------------------//
//- MATRIX4X4 operator* (MATRIX4X4, MATRIX4X4) ---------------------//
//------------------------------------------------------------------//
//- Description: Overloading the * operator.  This does the 'real' -//
//-				 matrix multiplication, and is pretty slow, though -//
//-				 not nearly as slow as the division operation      -//
//-				 though.										   -//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-					result= matrix1*matrix2;					   -//
//------------------------------------------------------------------//
MATRIX4X4 operator* (const MATRIX4X4 &m1, const MATRIX4X4 &m2)
{
	MATRIX4X4 result;

	result.matrix[0]= (m1.matrix[0]*m2.matrix[0])+(m1.matrix[4]*m2.matrix[1])+(m1.matrix[8]*m2.matrix[2])+(m1.matrix[12]*m2.matrix[3]);
	result.matrix[4]= (m1.matrix[0]*m2.matrix[4])+(m1.matrix[4]*m2.matrix[5])+(m1.matrix[8]*m2.matrix[6])+(m1.matrix[12]*m2.matrix[7]);
	result.matrix[8]= (m1.matrix[0]*m2.matrix[8])+(m1.matrix[4]*m2.matrix[9])+(m1.matrix[8]*m2.matrix[10])+(m1.matrix[12]*m2.matrix[11]);
	result.matrix[12]= (m1.matrix[0]*m2.matrix[12])+(m1.matrix[4]*m2.matrix[13])+(m1.matrix[8]*m2.matrix[14])+(m1.matrix[12]*m2.matrix[15]);
	
	result.matrix[1]= (m1.matrix[1]*m2.matrix[0])+(m1.matrix[5]*m2.matrix[1])+(m1.matrix[9]*m2.matrix[2])+(m1.matrix[13]*m2.matrix[3]);
	result.matrix[5]= (m1.matrix[1]*m2.matrix[4])+(m1.matrix[5]*m2.matrix[5])+(m1.matrix[9]*m2.matrix[6])+(m1.matrix[13]*m2.matrix[7]);
	result.matrix[9]= (m1.matrix[1]*m2.matrix[8])+(m1.matrix[5]*m2.matrix[9])+(m1.matrix[9]*m2.matrix[10])+(m1.matrix[13]*m2.matrix[11]);
	result.matrix[13]= (m1.matrix[1]*m2.matrix[12])+(m1.matrix[5]*m2.matrix[13])+(m1.matrix[9]*m2.matrix[14])+(m1.matrix[13]*m2.matrix[15]);

	result.matrix[2]= (m1.matrix[2]*m2.matrix[0])+(m1.matrix[6]*m2.matrix[1])+(m1.matrix[10]*m2.matrix[2])+(m1.matrix[14]*m2.matrix[3]);
	result.matrix[6]= (m1.matrix[2]*m2.matrix[4])+(m1.matrix[6]*m2.matrix[5])+(m1.matrix[10]*m2.matrix[6])+(m1.matrix[14]*m2.matrix[7]);
	result.matrix[10]= (m1.matrix[2]*m2.matrix[8])+(m1.matrix[6]*m2.matrix[9])+(m1.matrix[10]*m2.matrix[10])+(m1.matrix[14]*m2.matrix[11]);
	result.matrix[14]= (m1.matrix[2]*m2.matrix[12])+(m1.matrix[6]*m2.matrix[13])+(m1.matrix[10]*m2.matrix[14])+(m1.matrix[14]*m2.matrix[15]);

	result.matrix[3]= (m1.matrix[3]*m2.matrix[0])+(m1.matrix[7]*m2.matrix[1])+(m1.matrix[11]*m2.matrix[2])+(m1.matrix[15]*m2.matrix[3]);
	result.matrix[7]= (m1.matrix[3]*m2.matrix[4])+(m1.matrix[7]*m2.matrix[5])+(m1.matrix[11]*m2.matrix[6])+(m1.matrix[15]*m2.matrix[7]);
	result.matrix[11]= (m1.matrix[3]*m2.matrix[8])+(m1.matrix[7]*m2.matrix[9])+(m1.matrix[11]*m2.matrix[10])+(m1.matrix[15]*m2.matrix[11]);
	result.matrix[15]= (m1.matrix[3]*m2.matrix[12])+(m1.matrix[7]*m2.matrix[13])+(m1.matrix[11]*m2.matrix[14])+(m1.matrix[15]*m2.matrix[15]);

	return result;
}
