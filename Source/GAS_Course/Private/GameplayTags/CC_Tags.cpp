#include "GameplayTags/CC_Tags.h"

namespace CCTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, "CCTags.None");
	
	namespace SetByCaller
	{
		UE_DEFINE_GAMEPLAY_TAG(Projectile, "CCTags.SetByCaller.Projectile");
	}
	
	namespace CCAbilities
	{
		UE_DEFINE_GAMEPLAY_TAG(Death, "CCTags.CCAbilities.Death");
		UE_DEFINE_GAMEPLAY_TAG(ActivateOnGiven, "CCTags.CCAbilities.ActivateOnGiven");
		
		UE_DEFINE_GAMEPLAY_TAG(Primary, "CCTags.CCAbilities.Primary");
		UE_DEFINE_GAMEPLAY_TAG(Secondary, "CCTags.CCAbilities.Secondary");
		UE_DEFINE_GAMEPLAY_TAG(Tertiary, "CCTags.CCAbilities.Tertiary");
	}
	
	namespace CCEvents
	{
		UE_DEFINE_GAMEPLAY_TAG(KillScored, "CCTags.Events.KillScored");

		namespace Player
		{
			UE_DEFINE_GAMEPLAY_TAG(HitReact, "CCTags.CCEvents.Player.HitReact");
			UE_DEFINE_GAMEPLAY_TAG(Death, "CCTags.CCEvents.Player.Death");
		}
		
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG(HitReact, "CCTags.CCEvents.Enemy.HitReact");
			UE_DEFINE_GAMEPLAY_TAG(MeleeTraceHit, "CCTags.CCEvents.Enemy.MeleeTraceHit");
		}
	}
	
	namespace Events
	{
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG(EndAttack, "CCTags.Events.Enemy.EndAttack");
		}
	}
}
