//++ BulliT
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "game.h"
#include "player.h"
#include "hudtimer.h"

const char* g_CountdownSounds[] = {
	"vox/one",
	"vox/two",
	"vox/three",
	"vox/four",
	"vox/five",
	"vox/six",
	"vox/seven",
	"vox/eight",
	"vox/nine",
	"vox/ten"
};

HudTimer::HudTimer()
{
	m_fNextTimerUpdate = gpGlobals->time + 3.0; // Dont start timer directly.
	m_fLastTimeCheck = gpGlobals->time;
	m_fEffectiveTime = 0.0;
	m_pmp_timelimit = CVAR_GET_POINTER("mp_timelimit");
}

HudTimer::~HudTimer()
{
}

void HudTimer::Think()
{
	if ( !mp_clock.value )
		return;

	// Calculate effective time
	m_fEffectiveTime += gpGlobals->time - m_fLastTimeCheck;

	m_fLastTimeCheck = gpGlobals->time;

	if( m_fNextTimerUpdate <= m_fEffectiveTime )
	{
		// Sanity time check
		if( timelimit.value > 4320 )
        		CVAR_SET_FLOAT( "mp_timelimit", 4320 ); // Three days maximum

		// Write the time. (negative turns off timer on client)
		long lTime = (m_pmp_timelimit->value * 60) - m_fEffectiveTime;

		int r = 250;
		int g = 160;
		int b = 0;

		if( lTime > 0 )
		{
			char szTime[128];
			if( 86400 < lTime )
			{
				//More than one day. Print days, hour, minutes and seconds
				ldiv_t d1 = ldiv( lTime, 86400 );
				ldiv_t d2 = ldiv( d1.rem, 3600 );
				ldiv_t d3 = ldiv( d2.rem, 60 );
				sprintf( szTime, "%ldd %ldh %02ldm %02lds\n", d1.quot, d2.quot, d3.quot, d3.rem );
			}
			else if( 3600 < lTime )
			{
				// More than one hour. Print hour, minutes and seconds
				ldiv_t d1 = ldiv( lTime, 3600 );
				ldiv_t d2 = ldiv( d1.rem, 60 );
				sprintf( szTime, "%ldh %02ldm %02lds\n", d1.quot, d2.quot, d2.rem );
			}
			else if( 60 < lTime )
			{
				// More than one minute. Print minutes and seconds.
				ldiv_t d = ldiv( lTime, 60 );
				sprintf( szTime, "%ldm %02lds\n", d.quot, d.rem );
			}
			else
			{
				// Less than a minute left. Print seconds.
				sprintf( szTime, "%lds\n", lTime );
			}

			if (lTime <= 10 && lTime > 0)
			{
				r = 250;
				g = 0;
				b = 0;

				const char* countdownSound = g_CountdownSounds[lTime - 1];
				UTIL_SpeakAll(countdownSound);
			}

			UTIL_DrawHudMessageAll( CHAN_TIMER, Vector(r, g, b), Vector(0, 60, 0), szTime );
			m_fNextTimerUpdate += 1;
		}
	}
}
//-- Martin Webrant
