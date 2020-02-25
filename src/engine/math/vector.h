#ifndef ENGINE_MATH_VECTOR_I
#define ENGINE_MATH_VECTOR_I

#include <SDL_assert.h>
#include <math.h>

// To view macro expansion result comment includes and: gcc -E vector.h

#define GenerateVector(name, type)                                                                              \
	typedef struct Vector2D##name {                                                                             \
		type x;                                                                                                 \
		type y;                                                                                                 \
	} Vector2D##name;                                                                                           \
                                                                                                                \
	inline int engine_math_vector2d##name##_are_opposite(Vector2D##name *v1, Vector2D##name *v2) {              \
		SDL_assert(v1);                                                                                         \
		SDL_assert(v2);                                                                                         \
		return v1->x == -v2->x && v1->y == -v2->y;                                                              \
	};                                                                                                          \
	inline double engine_math_vector2d##name##_length(Vector2D##name *v) {                                      \
		SDL_assert(v);                                                                                          \
		return sqrt(v->x * v->x + v->y * v->y);                                                                 \
	};                                                                                                          \
	inline double engine_math_vector2d##name##_direction(Vector2D##name *v) {                                   \
		SDL_assert(v);                                                                                          \
		return atan(v->x / (double)v->y);                                                                       \
	};                                                                                                          \
	inline void engine_math_vector2d##name##_sub(Vector2D##name *v1, Vector2D##name *v2, Vector2D##name *out) { \
		SDL_assert(v1);                                                                                         \
		SDL_assert(v2);                                                                                         \
		SDL_assert(out);                                                                                        \
		out->x = v1->x - v2->x;                                                                                 \
		out->y = v1->y - v2->y;                                                                                 \
	};                                                                                                          \
	inline void engine_math_vector2d##name##_add(Vector2D##name *v1, Vector2D##name *v2, Vector2D##name *out) { \
		SDL_assert(v1);                                                                                         \
		SDL_assert(v2);                                                                                         \
		SDL_assert(out);                                                                                        \
		out->x = v1->x + v2->x;                                                                                 \
		out->y = v1->y + v2->y;                                                                                 \
	};                                                                                                          \
	inline double engine_math_vector2d##name##_distance(Vector2D##name *v1, Vector2D##name *v2) {               \
		SDL_assert(v1);                                                                                         \
		SDL_assert(v2);                                                                                         \
		Vector2D##name out;                                                                                     \
		engine_math_vector2d##name##_sub(v1, v2, &out);                                                         \
		return engine_math_vector2d##name##_length(&out);                                                       \
	};                                                                                                          \
	inline void engine_math_vector2d##name##_scale(Vector2D##name *v, type scalar, Vector2D##name *out) {       \
		SDL_assert(v);                                                                                          \
		SDL_assert(out);                                                                                        \
		out->x = v->x * scalar;                                                                                 \
		out->y = v->y * scalar;                                                                                 \
	};                                                                                                          \
	inline type engine_math_vector2d##name##_dot(Vector2D##name *v1, Vector2D##name *v2) {                      \
		SDL_assert(v1);                                                                                         \
		SDL_assert(v2);                                                                                         \
		return v1->x * v2->x + v1->y * v2->y;                                                                   \
	}

GenerateVector(i, int);

GenerateVector(ui, unsigned int);

GenerateVector(l, long);

GenerateVector(ul, unsigned long);

GenerateVector(f, float);

GenerateVector(d, double);

#endif
