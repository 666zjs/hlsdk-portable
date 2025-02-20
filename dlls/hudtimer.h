//++ BulliT
class HudTimer
{
	cvar_t* m_pmp_timelimit;
	float m_fNextTimerUpdate;
	float m_fLastTimeCheck;
	float m_fEffectiveTime;

public:
	HudTimer();
	virtual ~HudTimer();

	void Think();
};
//-- Martin Webrant
