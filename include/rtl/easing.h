#pragma once

namespace rtl { namespace easing
{
	typedef	float ( *function )( float );

	extern function default_easing;

	extern float linear( float t );
	extern float quadratic_in( float t );
	extern float quadratic_out( float t );
	extern float quadratic_in_out( float t );
	extern float cubic_in( float t );
	extern float cubic_out( float t );
	extern float cubic_in_out( float t );
	extern float quartic_in( float t );
	extern float quartic_out( float t );
	extern float quartic_in_out( float t );
	extern float quintic_in( float t );
	extern float quintic_out( float t );
	extern float quintic_in_out( float t );
	extern float sinusoidal_in( float t );
	extern float sinusoidal_out( float t );
	extern float sinusoidal_in_out( float t );
	extern float exponential_in( float t );
	extern float exponential_out( float t );
	extern float exponential_in_out( float t );
	extern float circular_in( float t );
	extern float circular_out( float t );
	extern float circular_in_out( float t );
	extern float elastic_in( float t );
	extern float elastic_out( float t );
	extern float elastic_in_out( float t );
	extern float back_in( float t );
	extern float back_out( float t );
	extern float back_in_out( float t );
	extern float bounce_in( float t );
	extern float bounce_out( float t );
	extern float bounce_in_out( float t );
} }