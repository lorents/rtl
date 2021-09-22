#include <rtl/easing.h>

#include <cmath>

namespace
{
	constexpr float cPI = 3.1415926535897932384626433832795f;
	constexpr float	cHalfPI = cPI * 0.5f;
	constexpr float	cQuarterPI = cPI * 0.25f;
	constexpr float	cTwoPI = cPI * 2.0f;
}

namespace rtl { namespace easing
{
	function default_easing = &linear;

	float linear( float t )
	{
		return t;
	}

	float quadratic_in( float t )
	{
		return t * t;
	}

	float quadratic_out( float t )
	{
		return t * ( 2.0f - t );
	}

	float quadratic_in_out( float t )
	{
		t *= 2.0f;

		if( t < 1.0f )
		{
			return 0.5f * t * t;
		}

		t -= 1.0f;
		return -0.5f * ( t * ( t - 2.0f ) - 1.0f );
	}

	float cubic_in( float t )
	{
		return t * t * t;
	}

	float cubic_out( float t )
	{
		t -= 1.0f;
		return ( t * t * t ) + 1.0f;
	}

	float cubic_in_out( float t )
	{
		t *= 2.0f;

		if( t < 1.0f )
		{
			return 0.5f * t * t * t;
		}

		t -= 2.0f;
		return 0.5f * (( t * t * t ) + 2.0f );
	}

	float quartic_in( float t )
	{
		return t * t * t * t;
	}

	float quartic_out( float t )
	{
		t -= 1;
		return 1 - ( t * t * t * t );
	}

	float quartic_in_out( float t )
	{
		t *= 2.0f;

		if( t < 1.0f )
		{
			return 0.5f * t * t * t * t;
		}

		t -= 2.0f;
		return -0.5f * (( t * t * t * t ) - 2.0f );
	}

	float quintic_in( float t )
	{
		return t * t * t * t * t;
	}

	float quintic_out( float t )
	{
		t -= 1.0f;
		return ( t * t * t * t * t ) + 1.0f;
	}

	float quintic_in_out( float t )
	{
		t *= 2.0f;

		if( t < 1.0f )
		{
			return 0.5f * t * t * t * t * t;
		}

		t -= 2.0f;
		return 0.5f * (( t * t * t * t * t ) + 2.0f );
	}

	float sinusoidal_in( float t )
	{
		return 1.0f - std::cos( t * cHalfPI );
	}

	float sinusoidal_out( float t )
	{
		return std::sin( t * cHalfPI );
	}

	float sinusoidal_in_out( float t )
	{
		return 0.5f * ( 1.0f - std::cos( cPI * t ));
	}

	float exponential_in( float t )
	{
		return ( t == 0.0f ) ? 0.0f : std::pow( 1024.0f, t - 1.0f );
	}

	float exponential_out( float t )
	{
		return ( t == 1.0f ) ? 1.0f : ( 1.0f - std::pow( 2.0f, -10.0f * t ));
	}

	float exponential_in_out( float t )
	{
		if( t == 0.0f )
		{
			return 0.0f;
		}
		else if( t == 1.0f )
		{
			return 1.0f;
		}

		t *= 2.0f;

		if( t < 1.0f )
		{
			return 0.5f * std::pow( 1024.0f, t - 1.0f );
		}
		return 0.5f * ( -std::pow( 2.0f, -10.0f * ( t - 1.0f )) + 2.0f );
	}

	float circular_in( float t )
	{
		return 1.0f - std::sqrt( 1.0f - ( t * t ));
	}

	float circular_out( float t )
	{
		t -= 1.0f;
		return std::sqrt( 1.0f - ( t * t ));
	}

	float circular_in_out( float t )
	{
		t *= 2.0f;

		if( t < 1.0f )
		{
			return -0.5f * ( std::sqrt( 1.0f - ( t * t )) - 1.0f );
		}
		t -= 2.0f;
		return 0.5f * ( std::sqrt( 1.0f - ( t * t )) + 1.0f );
	}

	float elastic_in( float t )
	{
		if( t == 0.0f )
		{
			return 0.0f;
		}
		else if( t == 1.0f )
		{
			return 1.0f;
		}

		t -= 1.0f;
		return -std::pow( 2.0f, 10.0f * t ) * std::sin(( t - 0.1f ) * cTwoPI * 2.5f );
	}

	float elastic_out( float t )
	{
		if( t == 0.0f )
		{
			return 0.0f;
		}
		else if( t == 1.0f )
		{
			return 1.0f;
		}

		return ( std::pow( 2.0f, -10.0f * t ) * std::sin(( t - 0.1f ) * cTwoPI * 2.5f )) + 1.0f;
	}

	float elastic_in_out( float t )
	{
		if( t == 0.0f )
		{
			return 0.0f;
		}
		else if( t == 1.0f )
		{
			return 1.0f;
		}

		t = ( t * 2.0f ) - 1.0f;

		if( t < 0.0f )
		{
			return -0.5f * std::pow( 2.0f, 10.0f * t ) * std::sin(( t - 0.1f ) * cTwoPI * 2.5f );
		}

		return ( std::pow( 2.0f, -10.0f * t ) * std::sin(( t - 0.1f ) * cTwoPI * 2.5f ) * 0.5f ) + 1.0f;
	}

	float back_in( float t )
	{
		return t * t * (( 2.70158f * t ) - 1.70158f );
	}

	float back_out( float t )
	{
		t -= 1.0f;
		return ( t * t * (( 2.70158f * t ) + 1.70158f )) + 1.0f;
	}

	float back_in_out( float t )
	{
		float	s = 1.70158f * 1.525f;

		t *= 2.0f;

		if( t < 1.0f )
		{
			return 0.5f * ( t * t * ((( s + 1.0f ) * t ) - s ));
		}

		t -= 2.0f;
		return 0.5f * ( t * t * ((( s + 1.0f ) * t ) + s ) + 2.0f );
	}

	float bounce_in( float t )
	{
		return 1.0f - bounce_out( 1.0f - t );
	}

	float bounce_out( float t )
	{
		if( t < ( 1.0f / 2.75f ))
		{
			return 7.5625f * t * t;
		}
		else if( t < ( 2.0f / 2.75f ))
		{
			t -= 1.5f / 2.75f;
			return ( 7.5625f * t * t ) + 0.75f;
		}
		else if( t < ( 2.5f / 2.75f ))
		{
			t -= 2.25f / 2.75f;
			return ( 7.562f * t * t ) + 0.9375f;
		}
		else
		{
			t -= 2.625f / 2.75f;
			return ( 7.5625f * t * t ) + 0.984375f;
		}
	}

	float bounce_in_out( float t )
	{
		if( t < 0.5f )
		{
			return bounce_in( t * 2.0f ) * 0.5f;
		}
		return ( bounce_out(( t * 2.0f ) - 1.0f ) * 0.5f ) + 0.5f;
	}

}}