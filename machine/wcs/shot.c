
#include <freewpc.h>

/**
 * Throw events for various shots that are determined
 * by more than just a single switch.
 */

/*
 * Throw goal_shot and striker_shot.
 *
 * The goal trough switch throws goal and disables
 * striker_shot until the ball is released.
 */

CALLSET_ENTRY (shot, sw_goal_trough)
{
	callset_invoke (goal_shot);
	free_timer_restart (TIM_GOAL_TROUGH_MADE, TIME_4S);
}

CALLSET_ENTRY (shot, dev_goal_popper_enter)
{
	if (!free_timer_test (TIM_GOAL_TROUGH_MADE))
	{
		callset_invoke (striker_shot);
	}
	else
	{
		free_timer_stop (TIM_GOAL_TROUGH_MADE);
	}
}

CALLSET_ENTRY (shot, dev_goal_popper_kick_attempt)
{
	free_timer_restart (TIM_GOAL_TROUGH_MADE, TIME_4S);
}

/*
 * Throw ramp events.
 *
 * A diverted left ramp should ignore the right ramp exit.
 */

void ramp_enter (void)
{
	sound_send (SND_ASTHMA_ATTACK);
}

CALLSET_ENTRY (shot, sw_l_ramp_enter)
{
	ramp_enter ();
}

CALLSET_ENTRY (shot, sw_r_ramp_enter)
{
	ramp_enter ();
}

CALLSET_ENTRY (shot, sw_l_ramp_diverted)
{
	/* TODO - be careful, cannot ask for more than 4s of this API */
	free_timer_restart (TIM_IGNORE_R_RAMP_EXIT, TIME_4S);
	callset_invoke (left_ramp_shot);
}

CALLSET_ENTRY (shot, sw_l_ramp_exit)
{
	callset_invoke (left_ramp_shot);
}

CALLSET_ENTRY (shot, sw_r_ramp_exit)
{
	if (!free_timer_test (TIM_IGNORE_R_RAMP_EXIT))
	{
		callset_invoke (right_ramp_shot);
	}
	else
	{
		free_timer_stop (TIM_IGNORE_R_RAMP_EXIT);
	}
}

/*
 * Throw kickback events.  Kickback upper throws
 * two different sub-events depending on direction:
 * 'enter' by default; 'exit' right after the lower
 * kickback.
 *
 * TODO - this does not work completely in multiball.
 */

CALLSET_ENTRY (shot, sw_kickback_upper)
{
	if (free_timer_test (TIM_KICKBACK_MADE))
	{
		free_timer_stop (TIM_KICKBACK_MADE);
		callset_invoke (sw_kickback_upper_exit);
	}
	else
	{
		callset_invoke (sw_kickback_upper_enter);
	}
}

CALLSET_ENTRY (shot, sw_kickback)
{
	free_timer_restart (TIM_KICKBACK_MADE, TIME_2S);
}

/*
 * Throw the left_loop event.  It is not counted
 * if certain switch closures occurred just prior to
 * the travel lane switch: bounceback off loop gate
 * back onto switch, etc.
 */

