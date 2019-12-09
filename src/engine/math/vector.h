#ifndef ENGINE_MATH_VECTOR_I
#define ENGINE_MATH_VECTOR_I


// TODO: Do this with macros and only float?
typedef struct Vector2Di {
	int x;
	int y;
} Vector2Di;

typedef Vector2Di Point2Di;

typedef struct Vector2Dui {
	unsigned int x;
	unsigned int y;
} Vector2Dui;

typedef Vector2Dui Point2Dui;

typedef struct Vector2Df {
	float x;
	float y;
} Vector2Df;

typedef Vector2Di Point2Dd;

typedef struct Vector2Dd {
	double x;
	double y;
} Vector2Dd;

typedef Vector2Di Point2Dd;

int engine_math_vector2di_are_opposite(Vector2Di *v1, Vector2Di *v2);
int engine_math_vector2di_are_parallel(Vector2Di *v1, Vector2Di *v2);
double engine_math_vector2di_length(Vector2Di *v);
double engine_math_Vector2di_distance(Vector2Di *v1, Vector2Di *v2);
void engine_math_vector2di_sub(Vector2Di *v1, Vector2Di *v2, Vector2Di *out);
void engine_math_vector2di_add(Vector2Di *v1, Vector2Di *v2, Vector2Di *out);
void engine_math_vector2di_scale(Vector2Di *v1, int scalar, Vector2Di *out);
int engine_math_vector2di_dot(Vector2Di *v1, Vector2Di *v2);

#endif
