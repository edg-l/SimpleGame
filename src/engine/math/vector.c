#include "vector.h"
#include <math.h>
#include <SDL_assert.h>


int engine_math_vector2di_are_opposite(Vector2Di *v1, Vector2Di *v2) {
	SDL_assert(v1);
	SDL_assert(v2);
	
	return v1->x == -v2->x && v1->y == -v2->y;
}

int engine_math_vector2di_are_parallel(Vector2Di *v1, Vector2Di *v2) {
	SDL_assert(v1);
	SDL_assert(v2);

}

double engine_math_vector2di_length(Vector2Di *v) {
	SDL_assert(v);

	return sqrt(v->x * v->x + v->y * v->y); 
}

double engine_math_Vector2di_distance(Vector2Di *v1, Vector2Di *v2) {
	SDL_assert(v1);
	SDL_assert(v2);

	Vector2Di out;
	engine_math_vector2di_sub(v1, v2, &out);
	return engine_math_vector2di_length(&out);
}

void engine_math_vector2di_sub(Vector2Di *v1, Vector2Di *v2, Vector2Di *out) {
	SDL_assert(v1);
	SDL_assert(v2);
	SDL_assert(out);

	out->x = v1->x - v2->x;
	out->y = v1->y - v2->y;
}

void engine_math_vector2di_add(Vector2Di *v1, Vector2Di *v2, Vector2Di *out) {
	SDL_assert(v1);
	SDL_assert(v2);
	SDL_assert(out);

	out->x = v1->x + v2->x;
	out->y = v1->y + v2->y;
}

void engine_math_vector2di_scale(Vector2Di *v, int scalar, Vector2Di *out) {
	SDL_assert(v1);
	SDL_assert(v2);
	SDL_assert(out);

	out->x = v->x * scalar;
	out->y = v->y * scalar;
}

int engine_math_vector2di_dot(Vector2Di *v1, Vector2Di *v2) {
	SDL_assert(v1);
	SDL_assert(v2);

	return v1->x * v2->x + v1->y * v2->y;
}
