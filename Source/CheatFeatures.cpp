#include "stdafx.h"

int Cheat::CheatFeatures::BoolOptionVectorPosition = 0; //0: Shop Box. 1: Circle
int Cheat::CheatFeatures::SpeedometerVectorPosition = 0;
int Cheat::CheatFeatures::PlayerOpacityInt = 250;
bool Cheat::CheatFeatures::HotkeyToggleBool = true;
bool Cheat::CheatFeatures::UseKMH = true;
bool Cheat::CheatFeatures::BlockScriptEvents = false;
bool Cheat::CheatFeatures::ShowBlockedScriptEventNotifications = true;
bool Cheat::CheatFeatures::ShowPlayerTagsPlayerList = true;
bool Cheat::CheatFeatures::ShowVehicleInfoAndPreview = true;
bool Cheat::CheatFeatures::ShowJoiningPlayersNotification = true;
std::chrono::steady_clock::time_point Cheat::CheatFeatures::PostInitScaleFormStart;


void Cheat::CheatFeatures::Looped()
{
	//Post Init Scaleform Banner Notification - Show for 10 seconds or until cheat GUI is opened
	if (!Cheat::GUI::CheatGUIHasBeenOpened && !(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - PostInitScaleFormStart).count() > 10))
	{ 
		auto ScaleformHandle = GRAPHICS::REQUEST_SCALEFORM_MOVIE(xorstr_("mp_big_message_freemode"));
		while (!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(ScaleformHandle)) { WAIT(0, false); }
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(ScaleformHandle, xorstr_("SHOW_SHARD_WASTED_MP_MESSAGE"));
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_STRING(xorstr_("<FONT FACE='$Font2'>GTAV Cheat"));
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_STRING(xorstr_("Cheat has been successfully initialized. Have fun!"));
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(5);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(ScaleformHandle, 255, 255, 255, 255, 0);
	}

	//Key Instructions, Cheat GUI
	if (!Cheat::GUI::CheatGUIHasBeenOpened)
	{
		std::string OpenGUIString = xorstr_("Press ") + Cheat::CheatFunctions::VirtualKeyCodeToString(Cheat::GUI::openKey) + xorstr_(" to open GUI");
		Cheat::GameFunctions::InstructionalKeysInit();
		Cheat::GameFunctions::InstructionsAdd(CheatFunctions::StringToChar(OpenGUIString), 80);
		Cheat::GameFunctions::InstructionsEnd();
	}


	//New Session Member Notification Feature
	Cheat::GameFunctions::CheckNewSessionMembersLoop();

	//Cursor GUI Navigation Feature
	Cheat::GameFunctions::CursorGUINavigationLoop();

	//Speedometer
	if (PED::IS_PED_IN_ANY_VEHICLE(PlayerPedID, 0)) 
	{
		std::ostringstream Speed;
		if (UseKMH)
		{
			Speed << Cheat::GameFunctions::MSToKMH(ENTITY::GET_ENTITY_SPEED(PED::GET_VEHICLE_PED_IS_IN(PlayerPedID, 0))) << xorstr_(" KM/H");
		}
		else
		{
			Speed << Cheat::GameFunctions::MSToMPH(ENTITY::GET_ENTITY_SPEED(PED::GET_VEHICLE_PED_IS_IN(PlayerPedID, 0))) << xorstr_(" MP/H");
		}
		if (SpeedometerVectorPosition == 1 || SpeedometerVectorPosition == 3) { Cheat::Speedometer(Speed.str()); }
		if (SpeedometerVectorPosition == 2 || SpeedometerVectorPosition == 3) { VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_IN(PlayerPedID, 0), CheatFunctions::StringToChar(Speed.str())); }
	}

	//Player Opacity
	ENTITY::SET_ENTITY_ALPHA(PlayerPedID, (PlayerOpacityInt), false);

	//Hotkey loops
	if (TeleportForward2mHotkey != NULL && HotkeyToggleBool)
	{
		if (GetAsyncKeyState(TeleportForward2mHotkey) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed())
		{
			Cheat::GameFunctions::MinimapNotification(xorstr_("Hotkey 'Teleport Forward (2m)' Triggered"));
			Vector3 Coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PlayerPedID, 0.0, 2.0, 0.0);
			int Handle = PlayerPedID;
			if (PED::IS_PED_IN_ANY_VEHICLE(Handle, 0)) { Handle = PED::GET_VEHICLE_PED_IS_IN(PlayerPedID, 0); }
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Handle, Coords.x, Coords.y, Coords.z, 0, 0, 1);
		}
	}
	if (MaxUpgradeVehicleHotKey != NULL && HotkeyToggleBool) { if (GetAsyncKeyState(MaxUpgradeVehicleHotKey) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed()) { Cheat::GameFunctions::MinimapNotification(xorstr_("Hotkey 'Max Upgrade Vehicle' Triggered")); Cheat::GameFunctions::MaxUpgradeVehicle(PED::GET_VEHICLE_PED_IS_USING(PlayerPedID)); } }
	if (BailToSinglePlayerHotkey != NULL && HotkeyToggleBool) { if (GetAsyncKeyState(BailToSinglePlayerHotkey) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed()) { Cheat::GameFunctions::MinimapNotification("Hotkey 'Bail To Single Player' Triggered"); NETWORK::SHUTDOWN_AND_LAUNCH_SINGLE_PLAYER_GAME(); } }
	if (SuicideHotkey != NULL && HotkeyToggleBool) { if (GetAsyncKeyState(SuicideHotkey) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed()) { Cheat::GameFunctions::MinimapNotification(xorstr_("Hotkey 'Suicide' Triggered")); PED::APPLY_DAMAGE_TO_PED(PlayerPedID, 300, true); } }
	if (GodModeToggleHotkey != NULL && HotkeyToggleBool)
	{
		if (GetAsyncKeyState(GodModeToggleHotkey) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed())
		{
			Cheat::GameFunctions::MinimapNotification(xorstr_("Hotkey 'Godmode Toggle' Triggered"));
			if (Cheat::CheatFeatures::GodmodeBool)
			{ Cheat::CheatFeatures::GodmodeBool = false; Cheat::GameFunctions::MinimapNotification(xorstr_("~g~Godmode Disabled")); }
			else  { Cheat::CheatFeatures::GodmodeBool = true; Cheat::GameFunctions::MinimapNotification(xorstr_("~g~Godmode Enabled")); }
		}
	}
	if (RepairAndCleanVehicleHotkey != NULL && HotkeyToggleBool) { if (GetAsyncKeyState(RepairAndCleanVehicleHotkey) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed()) { Cheat::GameFunctions::MinimapNotification(xorstr_("Hotkey 'Repair & Clean Vehicle' Triggered")); Cheat::GameFunctions::RepairAndCleanVehicle(); } }
	if (TeleportToWaypointHotkey != NULL && HotkeyToggleBool) { if (GetAsyncKeyState(TeleportToWaypointHotkey) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed()) { Cheat::GameFunctions::MinimapNotification(xorstr_("Hotkey 'Teleport to Waypoint' Triggered")); Cheat::GameFunctions::TeleportToWaypoint(); } }
	if (AntiCrashCameraHotkey != NULL && HotkeyToggleBool) { if (GetAsyncKeyState(AntiCrashCameraHotkey) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed()) { Cheat::GameFunctions::MinimapNotification(xorstr_("Hotkey 'Anti-Crash Camera' Triggered")); Cheat::GameFunctions::EnableDisableAntiCrashCamera(); } }

	GodmodeBool ? Godmode(true) : Godmode(false);
	NeverWantedBool ? NeverWanted(true) : NeverWanted(false);
	NoWeaponReloadBool ? NoWeaponReload() : NULL;
	SlowMotionBool ? SlowMotion(true) : SlowMotion(false);
	WorldBlackoutBool ? WorldBlackout(true) : WorldBlackout(false);
	GravityGunBool ? GravityGun() : NULL;
	HideHUDBool ? HideHUD() : NULL;
	NoGravityBool ? NoGravity(true) : NoGravity(false);
	WorldSnowLocalBool ? WorldSnowLocal(true) : WorldSnowLocal(false);
	AutoTeleportToWaypointBool ? AutoTeleportToWaypoint() : NULL;
	OneHitKillBool ? OneHitKill(true) : OneHitKill(false);
	PauseTimeBool ? PauseTime(true) : PauseTime(false);
	ExplosiveMeleeBool ? ExplosiveMelee() : NULL;
	OrbitalCannonCooldownBypassBool ? OrbitalCannonCooldownBypass() : NULL;
	ProtectionVoteKickBool ? ProtectionVoteKick(true) : ProtectionVoteKick(false);
	ProtectionFreezeBool ? ProtectionFreeze(true) : ProtectionFreeze(false);
	ProtectionWorldEventsBool ? ProtectionWorldEvents(true) : ProtectionWorldEvents(false);
	ProtectionVehicleBool ? ProtectionVehicle(true) : ProtectionVehicle(false);
	ProtectionAlterWantedLevelBool ? ProtectionAlterWantedLevel(true) : ProtectionAlterWantedLevel(false);
	ProtectionSessionTimeBool ? ProtectionSessionTime(true) : ProtectionSessionTime(false);
	ProtectionGiveRemoveWeaponsBool ? ProtectionGiveRemoveWeapons(true) : ProtectionGiveRemoveWeapons(false);
	ProtectionSessionWeatherBool ? ProtectionSessionWeather(true) : ProtectionSessionWeather(false);
	SuperJumpBool ? SuperJump() : NULL;
	PlayerForceFieldBool ? PlayerForceField() : NULL;
	FastRunBool ? FastRun(true) : FastRun(false);
	ShowFPSBool ? ShowFPS() : NULL;
	JumpAroundModeBool ? JumpAroundMode() : NULL;
	VehicleHornBoostBool ? VehicleHornBoost() : NULL;
	FakeWantedLevelBool ? FakeWantedLevel(true) : FakeWantedLevel(false);
	VehicleGodmodeBool ? VehicleGodmode(true) : VehicleGodmode(false);
	VehicleInvisibleBool ? VehicleInvisible(true) : VehicleInvisible(false);
	PlayerInvisibleBool ? PlayerInvisible(true) : PlayerInvisible(false);
	MobileRadioBool ? MobileRadio(true) : MobileRadio(false);
	WeaponRapidFireBool ? WeaponRapidFire() : NULL;
	PlayerIgnoredBool ? PlayerIgnored(true) : PlayerIgnored(false);
	NoClipBool ? NoClip() : NULL;
	RainbowVehicleBool ? RainbowVehicle() : NULL;
	TeleportGunBool ? TeleportGun() : NULL;
	DeleteGunBool ? DeleteGun() : NULL;
	UnlimitedSpecialAbilityBool ? UnlimitedSpecialAbility() : NULL;
	SpectatePlayerBool ? SpectatePlayer(true) : SpectatePlayer(false);
	NoRagdollAndSeatbeltBool ? NoRagdollAndSeatbelt(true) : NoRagdollAndSeatbelt(false);
	FreezeSelectedPlayerBool ? FreezeSelectedPlayer() : NULL;
	FreezeAllPlayersBool ? FreezeAllPlayers() : NULL;
	TriggerBotBool ? TriggerBot() : NULL;
	SuperBrakesBool ? SuperBrakes() : NULL;
	TinyPlayerBool ? TinyPlayer(true) : TinyPlayer(false);
	UnlimitedRocketBoostBool ? UnlimitedRocketBoost() : NULL;
	VehicleGunBool ? VehicleGun() : NULL;
	PlayerESPBool ? PlayerESP() : NULL;
	OffRadarBool ? OffRadar() : NULL;
	RevealPlayersBool ? RevealPlayers() : NULL;
	ExplodeLoopSelectedPlayerBool ? ExplodeLoopSelectedPlayer() : NULL;
	DriveOnWaterBool ? DriveOnWater() : NULL;
	SuperManBool ? SuperMan() : NULL;
	ShakeCamSelectedPlayerBool ? ShakeCamSelectedPlayer() : NULL;
	RainbowGunBool ? RainbowGun() : NULL;
	DisablePhoneBool ? DisablePhone() : NULL;
	NoIdleKickBool ? NoIdleKick() : NULL;
	BribeAuthoritiesBool ? BribeAuthorities() : NULL;
	MoneyDropBool ? MoneyDrop() : NULL;
	MoneyGunBool ? MoneyGun() : NULL;
	VehicleWeaponsBool ? VehicleWeapons() : NULL;
	AirstrikeGunBool ? AirstrikeGun() : NULL;
	SuperRunBool ? SuperRun() : NULL;
	CustomWeaponBulletsBool ? CustomWeaponBullets() : NULL;
	ShowSessionInformationBool ? ShowSessionInformation() : NULL;
	AutoGiveAllWeaponsBool ? AutoGiveAllWeapons() : NULL;
	FreeCamBool ? FreeCam(true) : FreeCam(false);
	CartoonGunBool ? CartoonGun() : NULL;
	EntityInformationGunBool ? EntityInformationGun() : NULL;
}


bool Cheat::CheatFeatures::GodmodeBool = false;
void Cheat::CheatFeatures::Godmode(bool toggle)
{
	ENTITY::SET_ENTITY_INVINCIBLE(PlayerPedID, toggle);
}

bool Cheat::CheatFeatures::NeverWantedBool = false;
void Cheat::CheatFeatures::NeverWanted(bool toggle)
{
	if (toggle)
	{
		PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PlayerID);
		PLAYER::SET_MAX_WANTED_LEVEL(0);
	}
	else
	{
		PLAYER::SET_MAX_WANTED_LEVEL(5);
	}
}

bool Cheat::CheatFeatures::NoWeaponReloadBool = false;
void Cheat::CheatFeatures::NoWeaponReload()
{
	Hash cur;
	if (WEAPON::GET_CURRENT_PED_WEAPON(PlayerPedID, &cur, 1))
	{
		if (WEAPON::IS_WEAPON_VALID(cur))
		{
			int maxAmmo;
			if (WEAPON::GET_MAX_AMMO(PlayerPedID, cur, &maxAmmo))
			{
				WEAPON::SET_PED_AMMO(PlayerPedID, cur, maxAmmo);
				maxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(PlayerPedID, cur, 1);
				if (maxAmmo > 0) { WEAPON::SET_AMMO_IN_CLIP(PlayerPedID, cur, maxAmmo); }
			}
		}
	}
}


bool Cheat::CheatFeatures::SlowMotionBool = false;
void Cheat::CheatFeatures::SlowMotion(bool toggle)
{
	if (toggle)
	{
		GAMEPLAY::SET_TIME_SCALE(0.2f);
	}
	else
	{
		GAMEPLAY::SET_TIME_SCALE(1.f);
	}
}


bool Cheat::CheatFeatures::WorldBlackoutBool = false;
void Cheat::CheatFeatures::WorldBlackout(bool toggle)
{
	GRAPHICS::_SET_BLACKOUT(toggle);
}


bool Cheat::CheatFeatures::GravityGunBool = false;
void Cheat::CheatFeatures::GravityGun()
{
	Entity EntityTarget;
	DWORD equippedWeapon;
	WEAPON::GET_CURRENT_PED_WEAPON(PlayerPedID, &equippedWeapon, 1);

	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	Vector3 dir = Cheat::GameFunctions::RotToDirection(&rot);
	Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PlayerPedID, 1);
	float spawnDistance = Cheat::GameFunctions::GetDistanceBetweenTwoVectors(&camPosition, &playerPosition);
	spawnDistance += 5;
	Vector3 spawnPosition = Cheat::GameFunctions::AddTwoVectors(&camPosition, &Cheat::GameFunctions::MultiplyVector(&dir, spawnDistance));

	Player tempPed = PlayerID;
	if (equippedWeapon == WeaponCombatPistol)
	{
		if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PlayerID, &EntityTarget) && GetAsyncKeyState(0x02))
		{
			Vector3 EntityTargetPos = ENTITY::GET_ENTITY_COORDS(EntityTarget, 0);
			PLAYER::DISABLE_PLAYER_FIRING(tempPed, true);
			if (ENTITY::IS_ENTITY_A_PED(EntityTarget) && PED::IS_PED_IN_ANY_VEHICLE(EntityTarget, 1))
			{
				EntityTarget = PED::GET_VEHICLE_PED_IS_IN(EntityTarget, 0);
			}

			Cheat::GameFunctions::RequestControl(EntityTarget);

			if (ENTITY::IS_ENTITY_A_VEHICLE(EntityTarget)) ENTITY::SET_ENTITY_HEADING(EntityTarget, ENTITY::GET_ENTITY_HEADING(tempPed));

			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(EntityTarget, spawnPosition.x, spawnPosition.y, spawnPosition.z, 0, 0, 0);

			if (GetAsyncKeyState(0x02) && Cheat::CheatFunctions::IsGameWindowFocussed())
			{
				ENTITY::SET_ENTITY_HEADING(EntityTarget, ENTITY::GET_ENTITY_HEADING(tempPed));
				ENTITY::APPLY_FORCE_TO_ENTITY(EntityTarget, 1, dir.x * 10000.0f, dir.y * 10000.0f, dir.z * 10000.0f, 0.0f, 0.0f, 0.0f, 0, 0, 1, 1, 0, 1);
				PLAYER::DISABLE_PLAYER_FIRING(tempPed, false);
			}
		}
		if (!PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PlayerID, &EntityTarget))
		{
			PLAYER::DISABLE_PLAYER_FIRING(tempPed, false);
		}
	}
}


bool Cheat::CheatFeatures::HideHUDBool = false;
void Cheat::CheatFeatures::HideHUD()
{
	UI::HIDE_HUD_AND_RADAR_THIS_FRAME();
}

bool Cheat::CheatFeatures::NoGravityBool = false;
void Cheat::CheatFeatures::NoGravity(bool toggle)
{
	if (toggle)
	{
		GAMEPLAY::SET_GRAVITY_LEVEL(3);
	}
	else
	{
		GAMEPLAY::SET_GRAVITY_LEVEL(0);
	}
}

bool Cheat::CheatFeatures::WorldSnowLocalBool = false;
void Cheat::CheatFeatures::WorldSnowLocal(bool toggle)
{
	globalHandle(262145).At(4723).As<BOOL>() = toggle;
}


bool Cheat::CheatFeatures::AutoTeleportToWaypointBool = false;
void Cheat::CheatFeatures::AutoTeleportToWaypoint()
{
	if (!UI::IS_PAUSE_MENU_ACTIVE()) { if (UI::IS_WAYPOINT_ACTIVE()) { Cheat::GameFunctions::TeleportToWaypoint(); } }
}



bool Cheat::CheatFeatures::OneHitKillBool = false;
void Cheat::CheatFeatures::OneHitKill(bool toggle)
{
	if (toggle)
	{
		PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PlayerPedID), 999999);
		PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PlayerPedID), 999999);
	}
	else
	{
		PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PlayerPedID), 1);
		PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PlayerPedID), 1);
	}
}

bool PauseTime_IsCurrentTimeSet = false;
int CurrentHourPauseTime;
int CurrentMinutePauseTime;
int CurrentSecondPauseTime;
bool Cheat::CheatFeatures::PauseTimeBool = false;
void Cheat::CheatFeatures::PauseTime(bool toggle)
{
	if (toggle)
	{
		if (!PauseTime_IsCurrentTimeSet)
		{
			PauseTime_IsCurrentTimeSet = true;
			CurrentHourPauseTime = TIME::GET_CLOCK_HOURS();
			CurrentMinutePauseTime = TIME::GET_CLOCK_MINUTES();
			CurrentSecondPauseTime = TIME::GET_CLOCK_SECONDS();
		}
		NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(CurrentHourPauseTime, CurrentMinutePauseTime, CurrentSecondPauseTime);
	}
	else
	{
		PauseTime_IsCurrentTimeSet = false;
	}
}


bool Cheat::CheatFeatures::ExplosiveMeleeBool = false;
void Cheat::CheatFeatures::ExplosiveMelee()
{
	Memory::set_value<int>({ 0x08, 0x10C8, 0x1F8 }, 0x2000u);
}

bool Cheat::CheatFeatures::OrbitalCannonCooldownBypassBool = false;
void Cheat::CheatFeatures::OrbitalCannonCooldownBypass()
{
	if (NETWORK::NETWORK_IS_SESSION_STARTED()) { STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY(xorstr_("MP0_ORBITAL_CANNON_COOLDOWN")), 0, 0); STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY(xorstr_("MP1_ORBITAL_CANNON_COOLDOWN")), 0, 0); }
}


bool Cheat::CheatFeatures::ProtectionVoteKickBool = false;
void Cheat::CheatFeatures::ProtectionVoteKick(bool toggle)
{
	GameHooking::defuseEvent(KICK_VOTES_EVENT, toggle);
	GameHooking::defuseEvent(REMOTE_SCRIPT_LEAVE_EVENT, toggle);
}

bool Cheat::CheatFeatures::ProtectionFreezeBool = false;
void Cheat::CheatFeatures::ProtectionFreeze(bool toggle)
{
	GameHooking::defuseEvent(RAGDOLL_REQUEST_EVENT, toggle);
	GameHooking::defuseEvent(INCIDENT_ENTITY_EVENT, toggle);
	GameHooking::defuseEvent(NETWORK_CLEAR_PED_TASKS_EVENT, toggle);
}


bool Cheat::CheatFeatures::ProtectionWorldEventsBool = false;
void Cheat::CheatFeatures::ProtectionWorldEvents(bool toggle)
{
	GameHooking::defuseEvent(NETWORK_PTFX_EVENT, toggle);
	GameHooking::defuseEvent(FIRE_EVENT, toggle);
	GameHooking::defuseEvent(EXPLOSION_EVENT, toggle);
	GameHooking::defuseEvent(START_PROJECTILE_EVENT, toggle);
	GameHooking::defuseEvent(RAGDOLL_REQUEST_EVENT, toggle);
	GameHooking::defuseEvent(CLEAR_AREA_EVENT, toggle);
	GameHooking::defuseEvent(CLEAR_RECTANGLE_AREA_EVENT, toggle);
	GameHooking::defuseEvent(NETWORK_SOUND_CAR_HORN_EVENT, toggle);
	GameHooking::defuseEvent(MODIFY_VEHICLE_LOCK_WORD_STATE_DATA, toggle);
	GameHooking::defuseEvent(BLOW_UP_VEHICLE_EVENT, toggle);
	GameHooking::defuseEvent(INFORM_SILENCED_GUNSHOT_EVENT, toggle);
	GameHooking::defuseEvent(PED_PLAY_PAIN_EVENT, toggle);
	GameHooking::defuseEvent(REQUEST_DETACHMENT_EVENT, toggle);
	GameHooking::defuseEvent(VEHICLE_COMPONENT_CONTROL_EVENT, toggle);
	GameHooking::defuseEvent(SCRIPTED_GAME_EVENT, toggle);
}

bool Cheat::CheatFeatures::ProtectionVehicleBool = false;
void Cheat::CheatFeatures::ProtectionVehicle(bool toggle)
{
	GameHooking::defuseEvent(VEHICLE_COMPONENT_CONTROL_EVENT, toggle);
	GameHooking::defuseEvent(NETWORK_SOUND_CAR_HORN_EVENT, toggle);
	GameHooking::defuseEvent(MODIFY_VEHICLE_LOCK_WORD_STATE_DATA, toggle);
	GameHooking::defuseEvent(BLOW_UP_VEHICLE_EVENT, toggle);
}

bool Cheat::CheatFeatures::ProtectionAlterWantedLevelBool = false;
void Cheat::CheatFeatures::ProtectionAlterWantedLevel(bool toggle)
{
	GameHooking::defuseEvent(ALTER_WANTED_LEVEL_EVENT, toggle);
}

bool Cheat::CheatFeatures::ProtectionSessionTimeBool = false;
void Cheat::CheatFeatures::ProtectionSessionTime(bool toggle)
{
	GameHooking::defuseEvent(GAME_CLOCK_EVENT, toggle);
}

bool Cheat::CheatFeatures::ProtectionGiveRemoveWeaponsBool = false;
void Cheat::CheatFeatures::ProtectionGiveRemoveWeapons(bool toggle)
{
	GameHooking::defuseEvent(GIVE_WEAPON_EVENT, toggle);
	GameHooking::defuseEvent(REMOVE_WEAPON_EVENT, toggle);
	GameHooking::defuseEvent(REMOVE_ALL_WEAPONS_EVENT, toggle);
}

bool Cheat::CheatFeatures::ProtectionSessionWeatherBool = false;
void Cheat::CheatFeatures::ProtectionSessionWeather(bool toggle)
{
	GameHooking::defuseEvent(GAME_WEATHER_EVENT, toggle);
}

bool Cheat::CheatFeatures::SuperJumpBool = false;
void Cheat::CheatFeatures::SuperJump()
{
	GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(PlayerID);
}

bool Cheat::CheatFeatures::PlayerForceFieldBool = false;
void Cheat::CheatFeatures::PlayerForceField()
{
	if(!NoRagdollAndSeatbeltBool) { NoRagdollAndSeatbeltBool = true; Cheat::GameFunctions::MinimapNotification(xorstr_("~o~Enabled No Ragdoll & Seatbelt feature to prevent your character from flying around")); }
	FIRE::ADD_EXPLOSION(ENTITY::GET_ENTITY_COORDS(PlayerPedID, 0).x, ENTITY::GET_ENTITY_COORDS(PlayerPedID, 0).y, ENTITY::GET_ENTITY_COORDS(PlayerPedID, 0).z, 7, 100.0f, false, true, 0.0f);
}

bool Cheat::CheatFeatures::FastRunBool = false;
void Cheat::CheatFeatures::FastRun(bool toggle)
{
	if (toggle)
	{
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(PlayerID, 1.39f);
	}
	else
	{
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(PlayerID, 1.00f);
	}
}


bool Cheat::CheatFeatures::ShowFPSBool = false;
void Cheat::CheatFeatures::ShowFPS()
{
	static int		iFrames = 0;
	static clock_t	clockFrames = clock();
	static float	iFps;
	iFrames++;
	clock_t dif = clock() - clockFrames;
	if (dif > 500)
	{
		iFps = iFrames / (dif / 1000.f);
		iFrames = 0;
		clockFrames = clock();
	}

	std::string str = std::to_string(iFps);
	while (str.size() > str.find(".")) { str.pop_back(); }
	std::string MessageString = xorstr_("FPS: ") + str;
	GUI::Drawing::Text(MessageString, { 255, 255, 255, 255 }, { 0.50f, 0.002f }, { 0.30f, 0.30f }, false);
}


bool Cheat::CheatFeatures::JumpAroundModeBool = false;
void Cheat::CheatFeatures::JumpAroundMode()
{
	const int ElementAmount = 10;
	const int ArrSize = ElementAmount * 2 + 2;

	Vehicle* vehs = new Vehicle[ArrSize];
	vehs[0] = ElementAmount;
	int VehFound = PED::GET_PED_NEARBY_VEHICLES(PlayerPedID, vehs);

	for (int i = 0; i < VehFound; i++)
	{
		int OffsetID = i * 2 + 2;
		if (vehs[OffsetID] != PED::GET_VEHICLE_PED_IS_IN(PlayerPedID, false))
		{
			if (VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(vehs[OffsetID]) == TRUE)
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(vehs[OffsetID], 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0);
			}
		}
	}
}

bool FreeCamFeaturedUsed = false;
bool Cheat::CheatFeatures::FreeCamBool = false;
void Cheat::CheatFeatures::FreeCam(bool toggle)
{
	static std::int32_t cam;
	static float dist;
	auto rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	auto coord = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 p_coord = { 0,0,0 };

	if (toggle)
	{
		FreeCamFeaturedUsed = true;
		if (!CAM::DOES_CAM_EXIST(cam)) {
			cam = CAM::CREATE_CAM(xorstr_("DEFAULT_SCRIPTED_CAMERA"), 1);
			CAM::SET_CAM_ROT(cam, rot.x, rot.y, rot.z, 0);
			CAM::SET_CAM_COORD(cam, coord.x, coord.y, coord.z);
		}

		CAM::RENDER_SCRIPT_CAMS(true, true, 700, 1, 1);
		CAM::SET_CAM_ACTIVE(cam, 1);
		CAM::SET_CAM_ROT(cam, rot.x, rot.y, rot.z, 0);

		p_coord = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PLAYER::PLAYER_PED_ID(), p_coord.x, p_coord.y, p_coord.z, 0, 0, 0);
		PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_PED_ID(), 1);
		UI::HIDE_HUD_AND_RADAR_THIS_FRAME();

		auto speed = .5f;
		if (GetAsyncKeyState(VK_LSHIFT))
		{
			speed += .3f;
		}

		if (GetAsyncKeyState(0x53))
		{
			speed /= -1;
			auto c = Cheat::GameFunctions::AddTwoVectors(&CAM::GET_CAM_COORD(cam), &Cheat::GameFunctions::MultiplyVector(&Cheat::GameFunctions::RotToDirection(&rot), speed));
			CAM::SET_CAM_COORD(cam, c.x, c.y, c.z);
		}

		if (GetAsyncKeyState(0x57))
		{
			auto c = Cheat::GameFunctions::AddTwoVectors(&CAM::GET_CAM_COORD(cam), &Cheat::GameFunctions::MultiplyVector(&Cheat::GameFunctions::RotToDirection(&rot), speed));
			CAM::SET_CAM_COORD(cam, c.x, c.y, c.z);
		}
	}
	else
	{
		if (FreeCamFeaturedUsed)
		{
			FreeCamFeaturedUsed = false;
			CAM::RENDER_SCRIPT_CAMS(0, 1, 10, 0, 0);
			CAM::SET_CAM_ACTIVE(cam, false);
			CAM::DESTROY_CAM(cam, true);
		}
	}
}


bool Cheat::CheatFeatures::VehicleHornBoostBool = false;
void Cheat::CheatFeatures::VehicleHornBoost()
{
	if (PLAYER::IS_PLAYER_PRESSING_HORN(PlayerID))
	{
		Vehicle Veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PlayerID), false);
		Cheat::GameFunctions::RequestControlOfEnt(Veh);
		VEHICLE::SET_VEHICLE_FORWARD_SPEED(Veh, 70);
	}
}

bool Cheat::CheatFeatures::FakeWantedLevelBool = false;
void Cheat::CheatFeatures::FakeWantedLevel(bool toggle)
{
	if (toggle)
	{
		GAMEPLAY::SET_FAKE_WANTED_LEVEL(6);
	}
	else
	{
		GAMEPLAY::SET_FAKE_WANTED_LEVEL(0);
	}
}

bool Cheat::CheatFeatures::VehicleGodmodeBool = false;
void Cheat::CheatFeatures::VehicleGodmode(bool toggle)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PlayerPedID);
	if (toggle)
	{
		GameFunctions::ChangeEntityInvincibilityState(veh, true);
	}
	else
	{
		GameFunctions::ChangeEntityInvincibilityState(veh, false);
	}
}

bool Cheat::CheatFeatures::VehicleInvisibleBool = false;
void Cheat::CheatFeatures::VehicleInvisible(bool toggle)
{
	if (toggle)
	{
		ENTITY::SET_ENTITY_VISIBLE(PED::GET_VEHICLE_PED_IS_USING(PlayerPedID), false, true);
	}
	else
	{
		ENTITY::SET_ENTITY_VISIBLE(PED::GET_VEHICLE_PED_IS_USING(PlayerPedID), true, true);
	}
}

bool Cheat::CheatFeatures::PlayerInvisibleBool = false;
void Cheat::CheatFeatures::PlayerInvisible(bool toggle)
{
	if (toggle)
	{
		ENTITY::SET_ENTITY_VISIBLE(PlayerPedID, false, true);
	}
	else
	{
		ENTITY::SET_ENTITY_VISIBLE(PlayerPedID, true, true);
	}
}

bool Cheat::CheatFeatures::MobileRadioBool = false;
void Cheat::CheatFeatures::MobileRadio(bool toggle)
{
	if (toggle)
	{
		AUDIO::SET_RADIO_TO_STATION_INDEX(AUDIO::GET_PLAYER_RADIO_STATION_INDEX());
		AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		AUDIO::SET_MOBILE_PHONE_RADIO_STATE(true);
	}
	else
	{
		AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
		AUDIO::SET_MOBILE_PHONE_RADIO_STATE(false);
	}
}


bool Cheat::CheatFeatures::WeaponRapidFireBool = false;
void Cheat::CheatFeatures::WeaponRapidFire()
{
	if (!PED::IS_PED_IN_ANY_VEHICLE(PlayerPedID, 1)) 
	{
		PLAYER::DISABLE_PLAYER_FIRING(PlayerPedID, 1);
		Vector3 gameplayCam = CAM::_GET_GAMEPLAY_CAM_COORDS();
		Vector3 gameplayCamRot = CAM::GET_GAMEPLAY_CAM_ROT(0);
		Vector3 gameplayCamDirection = Cheat::GameFunctions::RotationToDirection(gameplayCamRot);
		Vector3 startCoords = Cheat::GameFunctions::AddVector(gameplayCam, (Cheat::GameFunctions::MultiplyVector(gameplayCamDirection, 1.0f)));
		Vector3 endCoords = Cheat::GameFunctions::AddVector(startCoords, Cheat::GameFunctions::MultiplyVector(gameplayCamDirection, 500.0f));
		Hash weaponhash;
		WEAPON::GET_CURRENT_PED_WEAPON(PlayerPedID, &weaponhash, 1);
		if (CONTROLS::IS_CONTROL_PRESSED(2, 208) || GetKeyState(VK_LBUTTON) & 0x8000 && Cheat::CheatFunctions::IsGameWindowFocussed())
		{
			GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(startCoords.x, startCoords.y, startCoords.z, endCoords.x, endCoords.y, endCoords.z, 50, 1, weaponhash, PlayerPedID, 1, 1, 0xbf800000);
		}
	}
}


bool Cheat::CheatFeatures::PlayerIgnoredBool = false;
void Cheat::CheatFeatures::PlayerIgnored(bool toggle)
{
	PLAYER::SET_EVERYONE_IGNORE_PLAYER(PlayerPedID, toggle);
}

bool Cheat::CheatFeatures::NoClipBool = false;
void Cheat::CheatFeatures::NoClip()
{
	float x, y, z;
	float d = 0.999999f;

	Cheat::GameFunctions::GetCameraDirection(&x, &y, &z);

	if (!PED::IS_PED_ON_FOOT(PlayerPedID))
	{
		int currentCar = PED::GET_VEHICLE_PED_IS_IN(PlayerPedID, PLAYER::PLAYER_ID());
		Vector3 Pos = ENTITY::GET_ENTITY_COORDS(currentCar, false);
		Vector3 rotation = CAM::GET_GAMEPLAY_CAM_ROT(0);

		ENTITY::SET_ENTITY_COLLISION(currentCar, true, true);
		ENTITY::SET_ENTITY_ROTATION(currentCar, rotation.x, rotation.y, rotation.z, 2, 1);
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(currentCar, Pos.x, Pos.y, Pos.z, 0, 0, 0);
		if (ENTITY::DOES_ENTITY_EXIST(currentCar) && ENTITY::IS_ENTITY_A_VEHICLE(currentCar))
		{
			if (GetAsyncKeyState(0x57) && Cheat::CheatFunctions::IsGameWindowFocussed() || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, 232) && Cheat::GUI::ControllerInput)
			{
				ENTITY::SET_ENTITY_COLLISION(currentCar, false, false);
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(currentCar, Pos.x + (x * d), Pos.y + (y * d), Pos.z + (z * d), 0, 0, 0);
			}
		}
	}
	else
	{
		Vector3 Pos = ENTITY::GET_ENTITY_COORDS(PlayerPedID, false);

		Vector3 rotation = CAM::GET_GAMEPLAY_CAM_ROT(0);
		ENTITY::SET_ENTITY_COLLISION(PlayerPedID, true, true);
		ENTITY::SET_ENTITY_ROTATION(PlayerPedID, rotation.x, rotation.y, rotation.z, 2, 1);
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PlayerPedID, Pos.x, Pos.y, Pos.z, 0, 0, 0);
		if (GetAsyncKeyState(0x57) && Cheat::CheatFunctions::IsGameWindowFocussed() || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, 232) && Cheat::GUI::ControllerInput)
		{
			ENTITY::SET_ENTITY_COLLISION(PlayerPedID, false, false);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PlayerPedID, Pos.x + (x * d), Pos.y + (y * d), Pos.z + (z * d), 0, 0, 0);
		}
	}
}


bool Cheat::CheatFeatures::RainbowVehicleBool = false;
void Cheat::CheatFeatures::RainbowVehicle()
{
	int Red = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, 255),
	Green = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, 255),
	Blue = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, 255);
	VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(PED::GET_VEHICLE_PED_IS_USING(PlayerPedID), Red, Green, Blue);
	VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(PED::GET_VEHICLE_PED_IS_USING(PlayerPedID), Red, Green, Blue);
}

bool Cheat::CheatFeatures::TeleportGunBool = false;
void Cheat::CheatFeatures::TeleportGun()
{
	if (PED::IS_PED_SHOOTING(PlayerPedID))
	{
		Vector3 iCoord;
		if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PlayerPedID, &iCoord))
		{
			ENTITY::SET_ENTITY_COORDS(PlayerPedID, iCoord.x, iCoord.y, iCoord.z + 1, 0, 0, 1, 1);
			WAIT(0, false);
		}
	}
}

bool Cheat::CheatFeatures::EntityInformationGunBool = false;
void Cheat::CheatFeatures::EntityInformationGun()
{
	Entity AimedEntityHandle;
	if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PlayerID, &AimedEntityHandle))
	{
		Vector3 AimedEntityCoords = ENTITY::GET_ENTITY_COORDS(AimedEntityHandle, false);
		std::string AimedEntityHealth = xorstr_("Entity Health: ") + std::to_string(ENTITY::GET_ENTITY_HEALTH(AimedEntityHandle));
		std::string AimedEntityHash = xorstr_("Entity Hash: ") + std::to_string(ENTITY::GET_ENTITY_MODEL(AimedEntityHandle));
		Cheat::GUI::Drawing::Text(xorstr_("~bold~Aimed Entity Information"), { 255, 255, 255, 255, 0 }, { 0.500f, 0.380f }, { 0.35f, 0.35f }, false);
		Cheat::GUI::Drawing::Text(AimedEntityHash, { 255, 255, 255, 255, 0 }, { 0.500f, 0.400f }, { 0.35f, 0.35f }, false);
		Cheat::GUI::Drawing::Text(AimedEntityHealth, { 255, 255, 255, 255, 0 }, { 0.500f, 0.420f }, { 0.35f, 0.35f }, false);

		std::string EntityTypeMessageString;
		if (ENTITY::IS_ENTITY_A_PED(AimedEntityHandle)) { EntityTypeMessageString = xorstr_("Entity Type: Ped"); }
		else if (ENTITY::IS_ENTITY_A_VEHICLE(AimedEntityHandle)) { EntityTypeMessageString = xorstr_("Entity Type: Vehicle"); }
		else if (ENTITY::IS_ENTITY_AN_OBJECT(AimedEntityHandle)) { EntityTypeMessageString = xorstr_("Entity Type: Object"); }
		else { EntityTypeMessageString = xorstr_("Entity Type: Generic"); }
		Cheat::GUI::Drawing::Text(EntityTypeMessageString, { 255, 255, 255, 255, 0 }, { 0.500f, 0.440f }, { 0.35f, 0.35f }, false);
	}
}

bool Cheat::CheatFeatures::CartoonGunBool = false;
void Cheat::CheatFeatures::CartoonGun()
{
	if (PED::IS_PED_SHOOTING(PlayerPedID))
	{
		STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
		while (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("scr_rcbarry2")) { WAIT(0); }
		auto WeaponEntityHandle = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(PlayerPedID);
		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(WEAPON::GET_SELECTED_PED_WEAPON(PlayerPedID), &v0, &v1);
		GRAPHICS::USE_PARTICLE_FX_ASSET("scr_rcbarry2");
		GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_clown", WeaponEntityHandle, (v0.x - v1.x) / 2.0f + 0.7f, 0.f, 0.f, 0.f, 180.f, 0.f, 1.f, 1, 1, 1);
	}
}

bool Cheat::CheatFeatures::DeleteGunBool = false;
void Cheat::CheatFeatures::DeleteGun()
{
	Entity AimedAtEntity;
	DWORD equippedWeapon;
	WEAPON::GET_CURRENT_PED_WEAPON(PlayerPedID, &equippedWeapon, 1);
	if (equippedWeapon == WeaponSNSPistol || equippedWeapon == 0x88374054) //mk2 SNS Pistol
	{
		if (PED::IS_PED_SHOOTING(PlayerPedID))
		{
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PlayerID, &AimedAtEntity))
			{
				Cheat::GameFunctions::RequestControlOfEnt(AimedAtEntity);
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&AimedAtEntity);
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(AimedAtEntity, 1, 1);
				ENTITY::DELETE_ENTITY(&AimedAtEntity);
			}
		}
	}
}


bool Cheat::CheatFeatures::UnlimitedSpecialAbilityBool = false;
void Cheat::CheatFeatures::UnlimitedSpecialAbility()
{
	if (!NETWORK::NETWORK_IS_SESSION_STARTED()) { PLAYER::SPECIAL_ABILITY_FILL_METER(PlayerID, 1); }
}

bool Cheat::CheatFeatures::SpectatePlayerBool = false;
void Cheat::CheatFeatures::SpectatePlayer(bool toggle)
{
	if (toggle)
	{
		NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(PlayerPedID, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));
		std::string String = xorstr_("Spectating '") + (std::string)PLAYER::GET_PLAYER_NAME(selectedPlayer) + xorstr_("'");
		Cheat::GameFunctions::SubtitleNotification(CheatFunctions::StringToChar(String), 1);
	}
	else
	{
		NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(false, PlayerPedID);
	}
}

bool Cheat::CheatFeatures::NoRagdollAndSeatbeltBool = false;
void Cheat::CheatFeatures::NoRagdollAndSeatbelt(bool toggle)
{
	if (toggle)
	{
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PlayerPedID, false);
		PED::SET_PED_CAN_RAGDOLL(PlayerPedID, false);
		PED::SET_PED_CONFIG_FLAG(PlayerPedID, 32, true);
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PlayerPedID, false);
	}
	else
	{
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PlayerPedID, true);
		PED::SET_PED_CAN_RAGDOLL(PlayerPedID, true);
		PED::SET_PED_CONFIG_FLAG(PlayerPedID, 32, false);
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PlayerPedID, true);
	}
}


bool Cheat::CheatFeatures::FreezeSelectedPlayerBool = false;
void Cheat::CheatFeatures::FreezeSelectedPlayer()
{
	Ped Player = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::selectedPlayer);
	AI::CLEAR_PED_TASKS_IMMEDIATELY(Player);
	AI::CLEAR_PED_TASKS(Player);
	AI::CLEAR_PED_SECONDARY_TASK(Player);
}


bool Cheat::CheatFeatures::FreezeAllPlayersBool = false;
void Cheat::CheatFeatures::FreezeAllPlayers()
{
	for (int i = 0; i < 32; i++)
	{
		if (i != PlayerPedID)
		{
			Ped Player = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::selectedPlayer);
			AI::CLEAR_PED_TASKS_IMMEDIATELY(Player);
			AI::CLEAR_PED_TASKS(Player);
			AI::CLEAR_PED_SECONDARY_TASK(Player);
		}
	}
}


bool Cheat::CheatFeatures::TriggerBot_ShootNPCBool = false;
bool Cheat::CheatFeatures::TriggerBot_ShootPlayersBool = false;
bool Cheat::CheatFeatures::TriggerBotBool = false;
void Cheat::CheatFeatures::TriggerBot()
{
	Entity AimedAtEntity;
	if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PlayerID, &AimedAtEntity))
	{
		Vector3 BoneCoords;
		BoneCoords = PED::GET_PED_BONE_COORDS(AimedAtEntity, SKEL_Head, 0.1f, 0.0f, 0.0f);
		if (TriggerBot_ShootNPCBool && TriggerBot_ShootPlayersBool) { if (ENTITY::IS_ENTITY_A_PED(AimedAtEntity) && !ENTITY::IS_ENTITY_DEAD(AimedAtEntity) && ENTITY::GET_ENTITY_ALPHA(AimedAtEntity) == 255) { PED::SET_PED_SHOOTS_AT_COORD(PlayerPedID, BoneCoords.x, BoneCoords.y, BoneCoords.z, true); } }
		else if (TriggerBot_ShootNPCBool && !TriggerBot_ShootPlayersBool) { if (ENTITY::IS_ENTITY_A_PED(AimedAtEntity) && !ENTITY::IS_ENTITY_DEAD(AimedAtEntity) && !PED::IS_PED_A_PLAYER(AimedAtEntity) && ENTITY::GET_ENTITY_ALPHA(AimedAtEntity) == 255) { PED::SET_PED_SHOOTS_AT_COORD(PlayerPedID, BoneCoords.x, BoneCoords.y, BoneCoords.z, true); } }
		else if (TriggerBot_ShootPlayersBool && !TriggerBot_ShootNPCBool) { if (ENTITY::IS_ENTITY_A_PED(AimedAtEntity) && !ENTITY::IS_ENTITY_DEAD(AimedAtEntity) && PED::IS_PED_A_PLAYER(AimedAtEntity) && ENTITY::GET_ENTITY_ALPHA(AimedAtEntity) == 255) { PED::SET_PED_SHOOTS_AT_COORD(PlayerPedID, BoneCoords.x, BoneCoords.y, BoneCoords.z, true); } }
	}
}


bool Cheat::CheatFeatures::SuperBrakesBool = false;
void Cheat::CheatFeatures::SuperBrakes()
{
	if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlVehicleBrake)) { VEHICLE::SET_VEHICLE_FORWARD_SPEED(PED::GET_VEHICLE_PED_IS_IN(PlayerPedID, 0), 0.0f); }
}

bool Cheat::CheatFeatures::TinyPlayerBool = false;
void Cheat::CheatFeatures::TinyPlayer(bool toggle)
{
	PED::SET_PED_CONFIG_FLAG(PlayerPedID, 223, toggle);
}

bool Cheat::CheatFeatures::UnlimitedRocketBoostBool = false;
void Cheat::CheatFeatures::UnlimitedRocketBoost()
{
	if (PED::IS_PED_IN_ANY_VEHICLE(PlayerPedID, 0)) {
		Ped veh = PED::GET_VEHICLE_PED_IS_USING(PlayerPedID);
		VEHICLE::_SET_VEHICLE_ROCKET_BOOST_REFILL_TIME(veh, 0);
		VEHICLE::_SET_VEHICLE_ROCKET_BOOST_PERCENTAGE(veh, 100.0f);

		if (VEHICLE::_IS_VEHICLE_ROCKET_BOOST_ACTIVE(veh)) {
			Cheat::GameFunctions::SubtitleNotification("~o~PRESS SPACEBAR TO STOP BOOST", 100);
			if (GetAsyncKeyState(VK_SPACE) && Cheat::CheatFunctions::IsGameWindowFocussed()) {
				VEHICLE::_SET_VEHICLE_ROCKET_BOOST_ACTIVE(veh, false);
			}
		}
	}
}

char* Cheat::CheatFeatures::VehicleGun_VehicleNameChar;
bool Cheat::CheatFeatures::VehicleGunBool = false;
void Cheat::CheatFeatures::VehicleGun()
{
	Ped playerPed = PlayerPedID;
	if (PED::IS_PED_SHOOTING(playerPed))
	{
		float offset = 0;
		int vehmodel = GAMEPLAY::GET_HASH_KEY(VehicleGun_VehicleNameChar);
		STREAMING::REQUEST_MODEL(vehmodel);

		while (!STREAMING::HAS_MODEL_LOADED(vehmodel)) { WAIT(0); }

		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PlayerPedID, 0.0, 5.0, 0.0);

		if (STREAMING::IS_MODEL_IN_CDIMAGE(vehmodel) && STREAMING::IS_MODEL_A_VEHICLE(vehmodel))
		{
			Vector3 dim1, dim2;
			GAMEPLAY::GET_MODEL_DIMENSIONS(vehmodel, &dim1, &dim2);

			offset = dim2.y * 1.6;

			Vector3 dir = ENTITY::GET_ENTITY_FORWARD_VECTOR(playerPed);
			Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
			float rot = (ENTITY::GET_ENTITY_ROTATION(playerPed, 0)).z;
			Vector3 gameplayCam = CAM::_GET_GAMEPLAY_CAM_COORDS();
			Vector3 gameplayCamRot = CAM::GET_GAMEPLAY_CAM_ROT(0);
			Vector3 gameplayCamDirection = Cheat::GameFunctions::RotationToDirection(gameplayCamRot);
			Vector3 startCoords = Cheat::GameFunctions::AddVector(gameplayCam, (Cheat::GameFunctions::MultiplyVector(gameplayCamDirection, 10)));
			Vector3 endCoords = Cheat::GameFunctions::AddVector(gameplayCam, (Cheat::GameFunctions::MultiplyVector(gameplayCamDirection, 500.0f)));

			Vehicle veh = VEHICLE::CREATE_VEHICLE(vehmodel, pCoords.x + (dir.x * offset), pCoords.y + (dir.y * offset), startCoords.z, rot, 1, 1);
			ENTITY::SET_ENTITY_VISIBLE(veh, false, 0);

			ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, 0.0f, 500.0f, 2.0f + endCoords.z, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
		}
	}
}


bool Cheat::CheatFeatures::PlayerESPBool = false;
void Cheat::CheatFeatures::PlayerESP()
{
	if (NETWORK::NETWORK_GET_NUM_CONNECTED_PLAYERS() > 1)
	{
		for (int i = 1; i <= 32; i++) {
			if (PlayerID != i) {
				Vector3 entitylocation = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false);
				Vector3 top1world = { entitylocation.x + 0.3f, NULL, entitylocation.y + 0.3f, NULL, entitylocation.z + .8f, NULL };
				Vector3 top2world = { entitylocation.x - 0.3f, NULL, entitylocation.y + 0.3f, NULL, entitylocation.z + .8f, NULL };
				Vector3 top3world = { entitylocation.x + 0.3f, NULL, entitylocation.y - 0.3f, NULL, entitylocation.z + .8f, NULL };
				Vector3 top4world = { entitylocation.x - 0.3f, NULL, entitylocation.y - 0.3f, NULL, entitylocation.z + .8f, NULL };
				Vector3 bottom1world = { entitylocation.x + 0.3f, NULL, entitylocation.y + 0.3f, NULL, entitylocation.z - .8f, NULL };
				Vector3 bottom2world = { entitylocation.x - 0.3f, NULL, entitylocation.y + 0.3f, NULL, entitylocation.z - .8f, NULL };
				Vector3 bottom3world = { entitylocation.x + 0.3f, NULL, entitylocation.y - 0.3f, NULL, entitylocation.z - .8f, NULL };
				Vector3 bottom4world = { entitylocation.x - 0.3f, NULL, entitylocation.y - 0.3f, NULL, entitylocation.z - .8f, NULL };

				GRAPHICS::DRAW_LINE(top1world.x, top1world.y, top1world.z, top2world.x, top2world.y, top2world.z, 0, 0, 255, 255);
				GRAPHICS::DRAW_LINE(top2world.x, top2world.y, top2world.z, top4world.x, top4world.y, top4world.z, 0, 0, 255, 255);
				GRAPHICS::DRAW_LINE(top4world.x, top4world.y, top4world.z, top3world.x, top3world.y, top3world.z, 0, 0, 255, 255);
				GRAPHICS::DRAW_LINE(top1world.x, top1world.y, top1world.z, top3world.x, top3world.y, top3world.z, 0, 0, 255, 255);

				GRAPHICS::DRAW_LINE(bottom1world.x, bottom1world.y, bottom1world.z, bottom2world.x, bottom2world.y, bottom2world.z, 0, 0, 255, 255);
				GRAPHICS::DRAW_LINE(bottom2world.x, bottom2world.y, bottom2world.z, bottom4world.x, bottom4world.y, bottom4world.z, 0, 0, 255, 255);
				GRAPHICS::DRAW_LINE(bottom3world.x, bottom3world.y, bottom3world.z, bottom4world.x, bottom4world.y, bottom4world.z, 0, 0, 255, 255);
				GRAPHICS::DRAW_LINE(bottom3world.x, bottom3world.y, bottom3world.z, bottom1world.x, bottom1world.y, bottom1world.z, 0, 0, 255, 255);

				GRAPHICS::DRAW_LINE(top1world.x, top1world.y, top1world.z, bottom1world.x, bottom1world.y, bottom1world.z, 0, 0, 255, 255);
				GRAPHICS::DRAW_LINE(top2world.x, top2world.y, top2world.z, bottom2world.x, bottom2world.y, bottom2world.z, 0, 0, 255, 255);
				GRAPHICS::DRAW_LINE(top3world.x, top3world.y, top3world.z, bottom3world.x, bottom3world.y, bottom3world.z, 0, 0, 255, 255);
				GRAPHICS::DRAW_LINE(top4world.x, top4world.y, top4world.z, bottom4world.x, bottom4world.y, bottom4world.z, 0, 0, 255, 255);

				Vector3 locationOne = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false);
				Vector3 locationTwo = ENTITY::GET_ENTITY_COORDS(PlayerPedID, false);
				GRAPHICS::DRAW_LINE(locationOne.x, locationOne.y, locationOne.z, locationTwo.x, locationTwo.y, locationTwo.z, 0, 0, 255, 255);
			}
		}
	}
}


bool Cheat::CheatFeatures::OffRadarBool = false;
void Cheat::CheatFeatures::OffRadar()
{
	globalHandle(2425662).At(1 + (PlayerID * 421)).At(200).As<int>() = 1;
	globalHandle(2439138).At(70).As<int>() = NETWORK::GET_NETWORK_TIME();
}

bool Cheat::CheatFeatures::RevealPlayersBool = false;
void Cheat::CheatFeatures::RevealPlayers()
{
	globalHandle(2424073).At(1 + (PlayerID * 421)).At(203).As<int>() = 1;
	globalHandle(2437549).At(71).As<int>() = NETWORK::GET_NETWORK_TIME();
}

bool Cheat::CheatFeatures::ExplodeLoopSelectedPlayerBool = false;
void Cheat::CheatFeatures::ExplodeLoopSelectedPlayer()
{
	Vector3 SelectedPlayerPedCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::selectedPlayer), false);
	FIRE::ADD_EXPLOSION(SelectedPlayerPedCoords.x, SelectedPlayerPedCoords.y, SelectedPlayerPedCoords.z, 0, 0.0f, true, false, 10.0f);
}


bool Cheat::CheatFeatures::DriveOnWaterBool = false;
void Cheat::CheatFeatures::DriveOnWater()
{
	Player player = PlayerID;
	Ped playerPed = PlayerPedID;
	Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PlayerPedID, 0);
	DWORD model = ENTITY::GET_ENTITY_MODEL(veh);
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	float height = 0;
	WATER::_SET_CURRENT_INTENSITY(height);
	if ((!(VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(veh)))) && WATER::GET_WATER_HEIGHT_NO_WAVES(pos.x, pos.y, pos.z, &height)) {
		Object container = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 4.0, GAMEPLAY::GET_HASH_KEY("prop_container_ld2"), 0, 0, 1);
		if (ENTITY::DOES_ENTITY_EXIST(container) && height > -50.0f) {
			Vector3 pRot = ENTITY::GET_ENTITY_ROTATION(playerPed, 0);
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1)) pRot = ENTITY::GET_ENTITY_ROTATION(veh, 0);
			Cheat::GameFunctions::RequestControlOfEnt(container);
			ENTITY::SET_ENTITY_COORDS(container, pos.x, pos.y, height - 1.5f, 0, 0, 0, 1);
			ENTITY::SET_ENTITY_ROTATION(container, 0, 0, pRot.z, 0, 1);
			Vector3 containerCoords = ENTITY::GET_ENTITY_COORDS(container, 1);
			if (pos.z < containerCoords.z) {
				if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
					ENTITY::SET_ENTITY_COORDS(playerPed, pos.x, pos.y, containerCoords.z + 2.0f, 0, 0, 0, 1);
				}
				else {
					Cheat::GameFunctions::RequestControlOfEnt(veh);
					Vector3 vehc = ENTITY::GET_ENTITY_COORDS(veh, 1);
					ENTITY::SET_ENTITY_COORDS(veh, vehc.x, vehc.y, containerCoords.z + 2.0f, 0, 0, 0, 1);
				}
			}
		}
		else {
			Hash model = GAMEPLAY::GET_HASH_KEY(xorstr_("prop_container_ld2"));
			STREAMING::REQUEST_MODEL(model);
			while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
			container = OBJECT::CREATE_OBJECT(model, pos.x, pos.y, pos.z, 1, 1, 0);

			(container);
			ENTITY::FREEZE_ENTITY_POSITION(container, 1);
			ENTITY::SET_ENTITY_ALPHA(container, 0, 1);
			ENTITY::SET_ENTITY_VISIBLE(container, 0, 0);
		}
	}
	else {
		Object container = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 4.0, GAMEPLAY::GET_HASH_KEY("prop_container_ld2"), 0, 0, 1);
		if (ENTITY::DOES_ENTITY_EXIST(container)) {
			Cheat::GameFunctions::RequestControlOfEnt(container);
			ENTITY::SET_ENTITY_COORDS(container, 0, 0, -1000.0f, 0, 0, 0, 1);
			WAIT(10);
			ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&container);
			ENTITY::DELETE_ENTITY(&container);
			WATER::_RESET_CURRENT_INTENSITY();
		}
	}
}

bool Cheat::CheatFeatures::SuperManBool = false;
void Cheat::CheatFeatures::SuperMan()
{
	if(!Cheat::CheatFeatures::NoRagdollAndSeatbeltBool) { Cheat::CheatFeatures::NoRagdollAndSeatbeltBool = true; Cheat::GameFunctions::MinimapNotification(xorstr_("No Ragdoll & Seatbelt feature enabled for this feature")); }
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PlayerPedID, GAMEPLAY::GET_HASH_KEY(xorstr_("GADGET_PARACHUTE")), 1, 1);
	ENTITY::SET_ENTITY_INVINCIBLE(PlayerPedID, true);
	PED::SET_PED_TO_RAGDOLL_WITH_FALL(PlayerPedID, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0);

	if (ENTITY::IS_ENTITY_IN_AIR(PlayerPedID) && !PED::IS_PED_RAGDOLL(PlayerPedID))
	{
		// W key
		if (GetAsyncKeyState(0x57) && Cheat::CheatFunctions::IsGameWindowFocussed()) { Cheat::GameFunctions::ApplyForceToEntity(PlayerPedID, 0, 6, 0); }
		// S key
		if (GetAsyncKeyState(0x53) && Cheat::CheatFunctions::IsGameWindowFocussed()) { Cheat::GameFunctions::ApplyForceToEntity(PlayerPedID, 0, -6, 0); }
		//VK_SHIFT
		if (GetAsyncKeyState(VK_SHIFT) && Cheat::CheatFunctions::IsGameWindowFocussed()) { Cheat::GameFunctions::ApplyForceToEntity(PlayerPedID, 0, 0, 6); }
	}
}


bool Cheat::CheatFeatures::ShakeCamSelectedPlayerBool = false;
void Cheat::CheatFeatures::ShakeCamSelectedPlayer()
{
	Vector3 targetCords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::selectedPlayer), false);
	FIRE::ADD_EXPLOSION(targetCords.x, targetCords.y, targetCords.z, 4, 0.f, false, true, 1000.f);
}

bool Cheat::CheatFeatures::RainbowGunBool = false;
void Cheat::CheatFeatures::RainbowGun()
{
	for (int i = 0; i < sizeof(Cheat::GameArrays::WeaponsHashList) / sizeof(Cheat::GameArrays::WeaponsHashList[0]); i++)
	{
		if (WEAPON::HAS_PED_GOT_WEAPON(PlayerPedID, Cheat::GameArrays::WeaponsHashList[i], 0))
		{
			WEAPON::SET_PED_WEAPON_TINT_INDEX(PlayerPedID, Cheat::GameArrays::WeaponsHashList[i], rand() % 8);
		}
	}
}

bool Cheat::CheatFeatures::DisablePhoneBool = false;
void Cheat::CheatFeatures::DisablePhone()
{
	MOBILE::_DISABLE_PHONE_THIS_FRAME(true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_PHONE, true);
}


bool Cheat::CheatFeatures::NoIdleKickBool = false;
void Cheat::CheatFeatures::NoIdleKick()
{
	//globalHandle(1382674).At(609).As<int>() = 609;
}

bool Cheat::CheatFeatures::BribeAuthoritiesBool = false;
void Cheat::CheatFeatures::BribeAuthorities()
{
	globalHandle(2528542).At(4546).As<int>() = 5;
	globalHandle(2528542).At(4547).As<int>() = 1;
	globalHandle(2528542).At(4549).As<int>() = NETWORK::GET_NETWORK_TIME();
}

int Cheat::CheatFeatures::MoneyDropDelay = 50;
int Cheat::CheatFeatures::MoneyDropDelayPreviousTick;
bool Cheat::CheatFeatures::MoneyDropBool = false;
void Cheat::CheatFeatures::MoneyDrop()
{
	if (GetTickCount64() - MoneyDropDelayPreviousTick > MoneyDropDelay)
	{
		Hash PolyBag = GAMEPLAY::GET_HASH_KEY(xorstr_("p_poly_bag_01_s"));
		Vector3 pp = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::selectedPlayer), 0.0, 0.0, 1.0);
		STREAMING::REQUEST_MODEL(PolyBag);
		while (!STREAMING::HAS_MODEL_LOADED(PolyBag)) { WAIT(0); }
		if (STREAMING::HAS_MODEL_LOADED(PolyBag))
		{
			OBJECT::CREATE_AMBIENT_PICKUP(0xCE6FDD6B, pp.x, pp.y, pp.z, 0, 2500, PolyBag, false, true);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(PolyBag);
		}
		MoneyDropDelayPreviousTick = GetTickCount64();
	}
}


bool Cheat::CheatFeatures::MoneyGunBool = false;
void Cheat::CheatFeatures::MoneyGun()
{
	if (PED::IS_PED_SHOOTING(PlayerPedID))
	{
		Vector3 coords;
		if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PlayerPedID, &coords))
		{
			Hash PolyBag = GAMEPLAY::GET_HASH_KEY(xorstr_("p_poly_bag_01_s"));
			STREAMING::REQUEST_MODEL(PolyBag);
			while (!STREAMING::HAS_MODEL_LOADED(PolyBag)) { WAIT(0, false); }
			if (STREAMING::HAS_MODEL_LOADED(PolyBag)) { OBJECT::CREATE_AMBIENT_PICKUP(0xCE6FDD6B, coords.x, coords.y, coords.z + 1.f, 0, 2500, PolyBag, false, true); STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(PolyBag); }
		}	
	}
}



DWORD VehicleWeapons_LastTick = 0;
bool Cheat::CheatFeatures::VehicleWeapons_TankRounds = true;
bool Cheat::CheatFeatures::VehicleWeapons_VehicleRockets = false;
bool Cheat::CheatFeatures::VehicleWeapons_Fireworks = false;
bool Cheat::CheatFeatures::VehicleWeapons_DrawLaser = false;
bool Cheat::CheatFeatures::VehicleWeaponsBool = false;
void Cheat::CheatFeatures::VehicleWeapons()
{
	if (VehicleWeapons_DrawLaser) { Cheat::GameFunctions::DrawVehicleLasers(); }
	if (PLAYER::IS_PLAYER_PRESSING_HORN(PlayerID) && VehicleWeapons_LastTick + 150 < GetTickCount64() && PLAYER::IS_PLAYER_CONTROL_ON(PlayerID) && PED::IS_PED_IN_ANY_VEHICLE(PlayerPedID, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PlayerPedID);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket;
		if (VehicleWeapons_TankRounds) { VehicleWeapons_Fireworks = false; VehicleWeapons_VehicleRockets = false; weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_TANK"); }
		else if (VehicleWeapons_VehicleRockets) { VehicleWeapons_TankRounds = false; VehicleWeapons_Fireworks = false; weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET"); }
		else if (VehicleWeapons_Fireworks) { VehicleWeapons_VehicleRockets = false; VehicleWeapons_TankRounds = false;  weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_FIREWORK"); }

		if (VehicleWeapons_TankRounds || VehicleWeapons_VehicleRockets || VehicleWeapons_Fireworks)
		{
			if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket)) { WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0); while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket)) { WAIT(0); } }

			Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1f);
			Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1f);
			Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
			Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);
			GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z, coords0to.x, coords0to.y, coords0to.z, 250, 1, weaponAssetRocket, PlayerPedID, 1, 0, -1.0);
			GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z, coords1to.x, coords1to.y, coords1to.z, 250, 1, weaponAssetRocket, PlayerPedID, 1, 0, -1.0);
		}
		VehicleWeapons_LastTick = GetTickCount64();
	}
}


bool Cheat::CheatFeatures::AirstrikeGunBool = false;
void Cheat::CheatFeatures::AirstrikeGun()
{
	if (PED::IS_PED_SHOOTING(PlayerPedID))
	{
		Vector3 coords;
		WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PlayerPedID, &coords);
		if (coords.x != 0 || coords.y != 0 || coords.z != 0) {
			GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords.x, coords.y, coords.z + 30, coords.x, coords.y, coords.z, 250, 0, GAMEPLAY::GET_HASH_KEY(xorstr_("VEHICLE_WEAPON_SPACE_ROCKET")), 0, 1, 1, 500);
		}
	}
}


bool Cheat::CheatFeatures::SuperRunBool = false;
void Cheat::CheatFeatures::SuperRun()
{
	if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlSprint) && CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlScriptPadUp))
	{
		Ped ped = PlayerPedID;
		Vector3 offset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.f, 0.6f, 0.f);
		ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, 0.0f, 1.3f, 0.f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1.f);
		PLAYER::SET_PLAYER_SPRINT(PlayerID, true);
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(PlayerID, 1.59f);
	}
}

bool Cheat::CheatFeatures::CustomWeaponBullets_ValkyrieGun = true;
bool Cheat::CheatFeatures::CustomWeaponBullets_TankBullets = false;
bool Cheat::CheatFeatures::CustomWeaponBullets_RpgBullets = false;
bool Cheat::CheatFeatures::CustomWeaponBullets_FireworkBullets = false;
bool Cheat::CheatFeatures::CustomWeaponBulletsBool = false;
void Cheat::CheatFeatures::CustomWeaponBullets()
{
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	Vector3 dir = Cheat::GameFunctions::RotToDirection(&rot);
	Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PlayerPedID, 1);
	float spawnDistance = Cheat::GameFunctions::GetDistanceBetweenTwoVectors(&camPosition, &playerPosition);
	spawnDistance += 0.25;
	Vector3 spawnPosition = Cheat::GameFunctions::AddTwoVectors(&camPosition, &Cheat::GameFunctions::MultiplyVector(&dir, spawnDistance));

	float endDistance = Cheat::GameFunctions::GetDistanceBetweenTwoVectors(&camPosition, &playerPosition);
	endDistance += 1000;
	Vector3 endPosition = Cheat::GameFunctions::AddTwoVectors(&camPosition, &Cheat::GameFunctions::MultiplyVector(&dir, endDistance));

	if (PED::IS_PED_ON_FOOT(PlayerPedID) && PED::IS_PED_SHOOTING(PlayerPedID))
	{
		Hash weaponAssetRocket1 = GAMEPLAY::GET_HASH_KEY(xorstr_("VEHICLE_WEAPON_NOSE_TURRET_VALKYRIE"));
		Hash weaponAssetRocket2 = GAMEPLAY::GET_HASH_KEY(xorstr_("VEHICLE_WEAPON_TANK"));
		Hash weaponAssetRocket3 = GAMEPLAY::GET_HASH_KEY(xorstr_("WEAPON_VEHICLE_ROCKET"));
		Hash weaponAssetRocket4 = GAMEPLAY::GET_HASH_KEY(xorstr_("WEAPON_FIREWORK"));

		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket1) || !WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket2) || !WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket3) || !WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket4))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket1, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket1)) { WAIT(0); }
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket2, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket2)) { WAIT(0); }
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket3, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket3)) { WAIT(0); }
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket4, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket4)) { WAIT(0); }
		}
		if (CustomWeaponBullets_ValkyrieGun) { GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(spawnPosition.x, spawnPosition.y, spawnPosition.z, endPosition.x, endPosition.y, endPosition.z, 250, 1, weaponAssetRocket1, PlayerPedID, 1, 0, -1.0); }
		if (CustomWeaponBullets_TankBullets) { GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(spawnPosition.x, spawnPosition.y, spawnPosition.z, endPosition.x, endPosition.y, endPosition.z, 250, 1, weaponAssetRocket2, PlayerPedID, 1, 0, -1.0); }
		if (CustomWeaponBullets_RpgBullets) { GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(spawnPosition.x, spawnPosition.y, spawnPosition.z, endPosition.x, endPosition.y, endPosition.z, 250, 1, weaponAssetRocket3, PlayerPedID, 1, 0, -1.0); }
		if (CustomWeaponBullets_FireworkBullets) { GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(spawnPosition.x, spawnPosition.y, spawnPosition.z, endPosition.x, endPosition.y, endPosition.z, 250, 1, weaponAssetRocket4, PlayerPedID, 1, 0, -1.0); }
	}
}


bool Cheat::CheatFeatures::ShowSessionInformationBool = false;
void Cheat::CheatFeatures::ShowSessionInformation()
{
	Vector3 playerCoord = ENTITY::GET_ENTITY_COORDS(PlayerPedID, 0);
	std::string NumbConnectedPlayers;
	NumbConnectedPlayers = xorstr_("Connected Players: ") + std::to_string(NETWORK::NETWORK_GET_NUM_CONNECTED_PLAYERS());
	std::string xMsg = xorstr_(" X ") + std::to_string(playerCoord.x);
	std::string yMsg = xorstr_(" Y ") + std::to_string(playerCoord.y);
	std::string zMsg = xorstr_(" Z ") + std::to_string(playerCoord.z);

	Cheat::GUI::Drawing::Text(xorstr_("Local Player Coords"), Cheat::GUI::optionText, { 0.162f, 0.8100f }, { 0.25f, 0.25f }, false);
	Cheat::GUI::Drawing::Text(xMsg, Cheat::GUI::optionText, { 0.16f, 0.8225f }, { 0.25f, 0.25f }, false);
	Cheat::GUI::Drawing::Text(yMsg, Cheat::GUI::optionText, { 0.16f, 0.8350f }, { 0.25f, 0.25f }, false);
	Cheat::GUI::Drawing::Text(zMsg, Cheat::GUI::optionText, { 0.16f, 0.8475f }, { 0.25f, 0.25f }, false);
	if (NETWORK::NETWORK_IS_SESSION_STARTED()) { Cheat::GUI::Drawing::Text(NumbConnectedPlayers, Cheat::GUI::optionText, { 0.1615f, 0.8650f }, { 0.25f, 0.25f }, false); }
}


bool Cheat::CheatFeatures::AutoGiveAllWeaponsBool = false;
void Cheat::CheatFeatures::AutoGiveAllWeapons()
{
	if (PLAYER::IS_PLAYER_PLAYING(PlayerID)) 
	{ 
		for (int i = 0; i < (sizeof(Cheat::GameArrays::WeaponsHashList) / 4); i++)
		{
			if (!WEAPON::HAS_PED_GOT_WEAPON(PlayerPedID, Cheat::GameArrays::WeaponsHashList[i], false))
			{
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PlayerPedID, Cheat::GameArrays::WeaponsHashList[i], 9999, false);
			}
		}
	}
}