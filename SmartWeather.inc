#if defined _smart_weather_included
	#endinput
#endif
#define _smart_weather_included


#include <a_samp>
#include <YSI-Includes\YSI_Coding\y_hooks>
#include <SmartWheater/gz_shapes>

// Configuration (Configuração)
// Override these defaults before including this file to tune the system. (Altere estes valores antes de incluir este arquivo para configurar o sistema.)
#if !defined SW_MAX_ZONES
	#define SW_MAX_ZONES 32
#endif
#if !defined SW_UPDATE_INTERVAL
	#define SW_UPDATE_INTERVAL 500
#endif
#if !defined SW_ZONE_MOVE_SPEED
	#define SW_ZONE_MOVE_SPEED 0.75
#endif
#if !defined SW_RED_ZONE_CHANCE
	#define SW_RED_ZONE_CHANCE 4
#endif
#if !defined SW_ZONE_VISUAL_INTERVAL
	#define SW_ZONE_VISUAL_INTERVAL 1000
#endif
#if !defined SW_ZONE_FADE_DURATION
	#define SW_ZONE_FADE_DURATION 180000
#endif
#if !defined SW_ZONE_FADE_MIN_DURATION
	#define SW_ZONE_FADE_MIN_DURATION 120000
#endif
#if !defined SW_RED_ZONE_FADE_MIN_DURATION
	#define SW_RED_ZONE_FADE_MIN_DURATION 240000
#endif
#if !defined SW_RED_ZONE_FADE_MAX_DURATION
	#define SW_RED_ZONE_FADE_MAX_DURATION 360000
#endif
#if !defined SW_ZONE_FADE_INTERVAL
	#define SW_ZONE_FADE_INTERVAL 500
#endif
#if !defined SW_ZONE_RADIUS_SPEED
	#define SW_ZONE_RADIUS_SPEED 8.0
#endif
#if !defined SW_MAX_WEATHER_ID
	#define SW_MAX_WEATHER_ID 22
#endif
#if !defined SW_MAX_DISEASES
	#define SW_MAX_DISEASES 16
#endif
#if !defined SW_AUTO_ZONE_INTERVAL
	#define SW_AUTO_ZONE_INTERVAL 300000
#endif
#if !defined SW_AUTO_ZONE_MAX_ACTIVE
	#define SW_AUTO_ZONE_MAX_ACTIVE 3
#endif
#if !defined SW_AUTO_ZONE_MIN_RADIUS
	#define SW_AUTO_ZONE_MIN_RADIUS 150.0
#endif
#if !defined SW_AUTO_ZONE_MAX_RADIUS
	#define SW_AUTO_ZONE_MAX_RADIUS 350.0
#endif

#define SW_ZONE_LABEL_LENGTH 48
#define SW_ZONE_SHAPE_PARTS 32
#define SW_PLAYER_WETNESS_THRESHOLD 20.0
#define SW_PLAYER_MIN_BODY_TEMPERATURE 25.0
#define SW_PLAYER_MAX_BODY_TEMPERATURE 42.0

#if !defined SW_SURVIVAL_ENABLED
	#define SW_SURVIVAL_ENABLED 1
#endif
#if !defined SW_DEBUG
	#define SW_DEBUG 0
#endif

#define SW_INVALID_ZONE_ID -1
#define SW_WEATHER_DRY 0
#define SW_WEATHER_CLOUDY 7
#define SW_WEATHER_RAIN 8
#define SW_WEATHER_STORM 16
#define SW_ZONE_BLUE 0
#define SW_ZONE_LIGHT_BLUE 0
#define SW_ZONE_DARK_BLUE 1
#define SW_ZONE_RED 2
#if !defined SW_COLOR_ZONE_BLUE
#define SW_COLOR_ZONE_BLUE 0x4488FF44
#endif
#if !defined SW_COLOR_ZONE_LIGHT_BLUE
#define SW_COLOR_ZONE_LIGHT_BLUE 0x66BFFF44
#endif
#if !defined SW_COLOR_ZONE_DARK_BLUE
#define SW_COLOR_ZONE_DARK_BLUE 0x2244CC44
#endif
#if !defined SW_COLOR_ZONE_RED
#define SW_COLOR_ZONE_RED 0xFF202066
#endif
#define SW_DISEASE_FLU 0
#define SW_DISEASE_HYPOTHERMIA 1
#define SW_DISEASE_HEATSTROKE 2

enum
{
	SW_SEASON_SPRING,
	SW_SEASON_SUMMER,
	SW_SEASON_AUTUMN,
	SW_SEASON_WINTER
}

static SW_BaseWeather = SW_WEATHER_DRY;
static sw_season;
static SW_Timer = -1;
static SW_TransitionSeconds = 60;

static SW_ClockMinute;
static Float:SW_ClockScale;
static Float:SW_ClockRemainder;
static SW_LastClockUpdate;
static SW_LastAppliedClockMinute = -1;

static SW_ZoneCount;
static SW_ZoneActive[SW_MAX_ZONES];
static Float:SW_ZoneX[SW_MAX_ZONES];
static Float:SW_ZoneY[SW_MAX_ZONES];
static Float:SW_ZoneTargetX[SW_MAX_ZONES];
static Float:SW_ZoneTargetY[SW_MAX_ZONES];
static SW_ZoneMoveTick[SW_MAX_ZONES];
static SW_ZoneVisualTick[SW_MAX_ZONES];
static SW_ZoneSpeedChangeTick[SW_MAX_ZONES];
static Float:SW_ZoneSpeedFactor[SW_MAX_ZONES];
static Float:SW_ZoneSizeFactor[SW_MAX_ZONES];
static Float:SW_ZoneRequestedRadius[SW_MAX_ZONES];
static bool:SW_ZoneFading[SW_MAX_ZONES];
static SW_ZoneFadeStartTick[SW_MAX_ZONES];
static SW_ZoneFadeVisualTick[SW_MAX_ZONES];
static SW_ZoneFadeDuration[SW_MAX_ZONES];
static Float:SW_ZoneFadeStartRadius[SW_MAX_ZONES];
static Float:SW_ZoneRadius[SW_MAX_ZONES];
static Float:SW_ZoneTargetRadius[SW_MAX_ZONES];
static SW_ZoneWeather[SW_MAX_ZONES];
static SW_ZonePriority[SW_MAX_ZONES];
static SW_ZoneTypeId[SW_MAX_ZONES];
static SW_ZoneSeverityLevel[SW_MAX_ZONES];
static SW_ZoneLabel[SW_MAX_ZONES][SW_ZONE_LABEL_LENGTH];
static Float:SW_ZoneWetRate[SW_MAX_ZONES];
static Float:SW_ZoneTemperatureRate[SW_MAX_ZONES];
static Float:SW_ZoneWindX[SW_MAX_ZONES];
static Float:SW_ZoneWindY[SW_MAX_ZONES];
static Float:SW_ZoneWindStrength[SW_MAX_ZONES];
static SW_ZoneShapeZones[SW_MAX_ZONES][SW_ZONE_SHAPE_PARTS];
static SW_ZoneShapeCount[SW_MAX_ZONES];
static SW_ZoneShapeSeed[SW_MAX_ZONES];
static SW_ZoneAutoCreated[SW_MAX_ZONES];
static SW_ZoneShapeColor[SW_MAX_ZONES];
static SW_ZoneShapeSquares[SW_MAX_ZONES];
static SW_ZoneMergeEnabled[SW_MAX_ZONES];
static SW_ZoneMerged[SW_MAX_ZONES];
static SW_ZoneMergeColor[SW_MAX_ZONES];
static SW_ZoneCustomMergeColor[SW_MAX_ZONES];
static Float:SW_ZoneMergeMaxRadius[SW_MAX_ZONES];
static Float:SW_ZoneMergeMapLimit[SW_MAX_ZONES];
static bool:SW_AutoZonesEnabled;
static Float:SW_AutoZoneMinX;
static Float:SW_AutoZoneMaxX;
static Float:SW_AutoZoneMinY;
static Float:SW_AutoZoneMaxY;
static Float:SW_AutoZoneMinRadius = SW_AUTO_ZONE_MIN_RADIUS;
static Float:SW_AutoZoneMaxRadius = SW_AUTO_ZONE_MAX_RADIUS;
static SW_AutoZoneInterval = SW_AUTO_ZONE_INTERVAL;
static SW_AutoZoneMaxActive = SW_AUTO_ZONE_MAX_ACTIVE;
static SW_AutoZoneNextSpawn;

static sw_player_weather[MAX_PLAYERS];
static SW_PlayerTargetWeather[MAX_PLAYERS];
static SW_PlayerWeatherKnown[MAX_PLAYERS];
static SW_PlayerStepInterval[MAX_PLAYERS];
static SW_PlayerNextStep[MAX_PLAYERS];
static SW_PlayerClockKnown[MAX_PLAYERS];
static SW_PlayerZone[MAX_PLAYERS];
static SW_PlayerZoneKnown[MAX_PLAYERS];
static SW_PlayerLocalWeather[MAX_PLAYERS];
static SW_PlayerGlobalWeather[MAX_PLAYERS];
static SW_PlayerGlobalKnown[MAX_PLAYERS];
static Float:SW_PlayerWetnessValue[MAX_PLAYERS];
static Float:SW_PlayerBodyTemperatureValue[MAX_PLAYERS];
static SW_PlayerSurvivalTick[MAX_PLAYERS];
static SW_PlayerDamageTick[MAX_PLAYERS];
static SW_PlayerDiseaseState[MAX_PLAYERS][SW_MAX_DISEASES];
static SW_PlayerDiseasesEnabled[MAX_PLAYERS];
static SW_SurvivalActive = SW_SURVIVAL_ENABLED;
static SW_DiseasesActive;
static Float:SW_HypothermiaDamage = 1.0;
static SW_HypothermiaInterval = 5000;
static Float:SW_WindX;
static Float:SW_WindY;
static Float:SW_WindStrength;
static Float:SW_WindAngle;
static SW_NextWindChange;

forward SW_Update();
forward OnWeatherChange(oldweather, newweather);
forward OnSeasonChange(seasonid);
forward OnPlayerEnterStorm(playerid);
forward OnPlayerWeatherChange(playerid, oldweather, newweather, Float:gripfactor);
forward OnWeatherZoneMerge(zoneid, absorbedid, Float:x, Float:y);
forward OnPlayerEnterWeatherZone(playerid, zoneid, typeid, weatherid);
forward OnPlayerLeaveWeatherZone(playerid, zoneid, typeid, weatherid);
forward OnPlayerWeatherBlend(playerid, globalweather, localweather, effectiveweather);
forward OnWeatherWindChange(zoneid, Float:windx, Float:windy, Float:strength);
forward OnPlayerSurvivalUpdate(playerid, Float:wetness, Float:bodytemperature, bool:iswet);
forward OnPlayerDiseaseChange(playerid, diseaseid, bool:active);

// Shared player checks (Validações compartilhadas de jogadores)
stock bool:sw_player_valid(playerid) {
	return playerid >= 0 && playerid < MAX_PLAYERS
		&& IsPlayerConnected(playerid);
}

// Wind (Vento)
stock sw_wind_change() {
	new Float:angle = float(random(360));
	SW_WindAngle = angle;
	SW_WindX = floatcos(angle, degrees);
	SW_WindY = floatsin(angle, degrees);
	SW_WindStrength = 0.25 + (float(random(76)) / 100.0);
	SW_NextWindChange = GetTickCount() + 30000 + random(30001);

	CallLocalFunction("OnWeatherWindChange", "ifff",
		SW_INVALID_ZONE_ID, SW_WindX, SW_WindY, SW_WindStrength);

	for(new zoneid = 0; zoneid < SW_MAX_ZONES; zoneid++) {
		if(SW_ZoneActive[zoneid] && !SW_ZoneFading[zoneid]) {
			sw_wind_update_zone(zoneid);
		}
	}
	return true;
}

stock sw_wind_update_zone(zoneid) {
	new Float:angle = SW_WindAngle + float(random(31) - 15);
	SW_ZoneWindX[zoneid] = floatcos(angle, degrees);
	SW_ZoneWindY[zoneid] = floatsin(angle, degrees);
	if(SW_ZoneSeverityLevel[zoneid] == SW_ZONE_RED) {
		SW_ZoneWindStrength[zoneid] = 0.80 + (float(random(21)) / 100.0);
	} else {
		SW_ZoneWindStrength[zoneid] = 0.10
			+ (SW_WindStrength * 0.55) + (float(random(16)) / 100.0);
		if(SW_ZoneWindStrength[zoneid] > 0.75) SW_ZoneWindStrength[zoneid] = 0.75;
	}

	CallLocalFunction("OnWeatherWindChange", "ifff",
		zoneid, SW_ZoneWindX[zoneid], SW_ZoneWindY[zoneid],
		SW_ZoneWindStrength[zoneid]);
	return true;
}

// Timer lifecycle (Ciclo de vida do timer)
stock SW_Init() {
	if(SW_Timer != -1) {
		return true;
	}

	SW_LastClockUpdate = GetTickCount();
	sw_wind_change();
	SW_Timer = SetTimer("SW_Update", SW_UPDATE_INTERVAL, true);

	if(SW_Timer == 0) {
		SW_Timer = -1;
		#if SW_DEBUG
		print("** [SmartWeather] The update timer couldn't be started.");
		#endif
		return false;
	}
	return true;
}

stock SW_Exit() {
	if(SW_Timer == -1) {
		return true;
	}

	KillTimer(SW_Timer);
	SW_Timer = -1;
	return true;
}

// Global weather and clock settings (Configurações globais de clima e relógio)
stock SW_Weather(weatherid, transition_seconds = 60) {
	if(weatherid < 0 || weatherid > SW_MAX_WEATHER_ID) {
		#if SW_DEBUG
		printf("** [SmartWeather] Invalid weather ID: %d (expected 0-%d).", weatherid, SW_MAX_WEATHER_ID);
		#endif
		return false;
	}

	if(transition_seconds < 0) {
		#if SW_DEBUG
		print("** [SmartWeather] The transition duration cannot be negative.");
		#endif
		return false;
	}

	if(SW_BaseWeather != weatherid) {
		new old_weather = SW_BaseWeather;
		SW_BaseWeather = weatherid;
		CallLocalFunction("OnWeatherChange", "ii", old_weather, weatherid);
	}

	SW_TransitionSeconds = transition_seconds;
	return true;
}

stock SW_Season(seasonid) {
	if(seasonid < SW_SEASON_SPRING || seasonid > SW_SEASON_WINTER) {
		#if SW_DEBUG
		printf("** [SmartWeather] Invalid season ID: %d.", seasonid);
		#endif
		return false;
	}

	if(sw_season != seasonid) {
		sw_season = seasonid;
		CallLocalFunction("OnSeasonChange", "i", seasonid);
	}
	return true;
}

stock SW_Clock(hour, minute, Float:game_minutes_per_real_minute = 0.0) {
	if(hour < 0 || hour > 23 || minute < 0 || minute > 59
		|| game_minutes_per_real_minute < 0.0) {
		#if SW_DEBUG
		print("** [SmartWeather] Invalid clock time or scale.");
		#endif
		return false;
	}

	// A zero scale keeps the clock fixed at the supplied time. (Uma escala zero mantém o relógio no horário informado.)
	SW_ClockMinute = (hour * 60) + minute;
	SW_ClockScale = game_minutes_per_real_minute;
	SW_ClockRemainder = 0.0;
	SW_LastClockUpdate = GetTickCount();
	SW_LastAppliedClockMinute = -1;
	sw_time();
	return true;
}

// Zone creation and configuration (Criação e configuração de zonas)
stock SW_Zone(Float:x, Float:y, Float:radius, weatherid, priority = 0) {
	if(SW_ZoneCount >= SW_MAX_ZONES) {
		#if SW_DEBUG
		printf("** [SmartWeather] The zone limit has been reached (%d).", SW_MAX_ZONES);
		#endif
		return SW_INVALID_ZONE_ID;
	}

	if(radius <= 0.0 || weatherid < 0 || weatherid > SW_MAX_WEATHER_ID) {
		#if SW_DEBUG
		print("** [SmartWeather] Invalid radius or weather ID for zone.");
		#endif
		return SW_INVALID_ZONE_ID;
	}

	new zone_id = 0;
	while(zone_id < SW_MAX_ZONES && SW_ZoneActive[zone_id]) {
		zone_id++;
	}

	if(zone_id >= SW_MAX_ZONES) {
		return SW_INVALID_ZONE_ID;
	}

	SW_ZoneActive[zone_id] = true;
	SW_ZoneCount++;
	new tick = GetTickCount();
	SW_ZoneX[zone_id] = x;
	SW_ZoneY[zone_id] = y;
	SW_ZoneTargetX[zone_id] = x;
	SW_ZoneTargetY[zone_id] = y;
	SW_ZoneMoveTick[zone_id] = tick;
	SW_ZoneVisualTick[zone_id] = tick;
	SW_ZoneSpeedChangeTick[zone_id] = tick + 20000 + random(40001);
	SW_ZoneSpeedFactor[zone_id] = 0.35 + (float(random(106)) / 100.0);
	SW_ZoneSizeFactor[zone_id] = 1.0;
	SW_ZoneRadius[zone_id] = radius;
	SW_ZoneTargetRadius[zone_id] = radius;
	SW_ZoneRequestedRadius[zone_id] = radius;
	SW_ZoneFading[zone_id] = false;
	SW_ZoneFadeStartTick[zone_id] = 0;
	SW_ZoneFadeVisualTick[zone_id] = 0;
	SW_ZoneFadeDuration[zone_id] = 0;
	SW_ZoneFadeStartRadius[zone_id] = radius;
	SW_ZoneWeather[zone_id] = weatherid;
	SW_ZonePriority[zone_id] = priority;
	SW_ZoneTypeId[zone_id] = 0;
	SW_ZoneSeverityLevel[zone_id] = SW_ZONE_BLUE;
	SW_ZoneLabel[zone_id][0] = '\0';
	SW_ZoneWetRate[zone_id] = 0.0;
	SW_ZoneTemperatureRate[zone_id] = 0.0;
	sw_wind_update_zone(zone_id);
	SW_ZoneShapeCount[zone_id] = 0;
	SW_ZoneShapeSeed[zone_id] = (tick + (zone_id * 97)) % 360;
	SW_ZoneAutoCreated[zone_id] = false;
	if(SW_ZoneShapeSeed[zone_id] < 0) {
		SW_ZoneShapeSeed[zone_id] += 360;
	}
	SW_ZoneMergeEnabled[zone_id] = false;
	SW_ZoneMerged[zone_id] = false;
	SW_ZoneMergeColor[zone_id] = 0;
	SW_ZoneMergeMaxRadius[zone_id] = radius;
	SW_ZoneMergeMapLimit[zone_id] = 0.0;
	return zone_id;
}

// Call after SW_Init(); the bounds define where zone centers may spawn.
// (Chame após SW_Init(); os limites definem onde o centro das zonas pode surgir.)
// SW_AutoZones(true, min_x, max_x, min_y, max_y);
// SW_AutoZones(false) stops new spawns but leaves existing zones active.
// (SW_AutoZones(false) interrompe novas criações, sem remover zonas existentes.)
stock SW_AutoZones(bool:enabled, Float:min_x = 0.0, Float:max_x = 0.0,
	Float:min_y = 0.0, Float:max_y = 0.0,
	Float:min_radius = SW_AUTO_ZONE_MIN_RADIUS,
	Float:max_radius = SW_AUTO_ZONE_MAX_RADIUS,
	interval = SW_AUTO_ZONE_INTERVAL, max_active_zones = SW_AUTO_ZONE_MAX_ACTIVE) {
	if(!enabled) {
		SW_AutoZonesEnabled = false;
		return true;
	}

	if(min_x > max_x || min_y > max_y || min_radius <= 0.0
		|| max_radius < min_radius || interval < 1
		|| max_active_zones < 1 || max_active_zones > SW_MAX_ZONES) {
		#if SW_DEBUG
		print("** [SmartWeather] Invalid automatic zone settings.");
		#endif
		return false;
	}

	SW_AutoZoneMinX = min_x;
	SW_AutoZoneMaxX = max_x;
	SW_AutoZoneMinY = min_y;
	SW_AutoZoneMaxY = max_y;
	SW_AutoZoneMinRadius = min_radius;
	SW_AutoZoneMaxRadius = max_radius;
	SW_AutoZoneInterval = interval;
	SW_AutoZoneMaxActive = max_active_zones;
	SW_AutoZoneNextSpawn = GetTickCount();
	SW_AutoZonesEnabled = true;
	return true;
}

stock SW_ZoneType(zoneid, typeid, const label[]) {
	if(zoneid < 0 || zoneid >= SW_MAX_ZONES || !SW_ZoneActive[zoneid]
		|| SW_ZoneFading[zoneid]) {
		return false;
	}

	SW_ZoneTypeId[zoneid] = typeid;
	format(SW_ZoneLabel[zoneid], SW_ZONE_LABEL_LENGTH, "%s", label);
	return true;
}

stock SW_ZoneSurvival(zoneid, Float:wetness_rate, Float:temperature_rate = 0.0) {
	if(zoneid < 0 || zoneid >= SW_MAX_ZONES || !SW_ZoneActive[zoneid]
		|| SW_ZoneFading[zoneid]
		|| wetness_rate < -100.0 || temperature_rate < -10.0
		|| temperature_rate > 10.0) {
		return false;
	}

	SW_ZoneWetRate[zoneid] = wetness_rate;
	SW_ZoneTemperatureRate[zoneid] = temperature_rate;
	return true;
}

// Survival and disease settings (Configurações de sobrevivência e doenças)
stock SW_Survival(bool:enabled = true, Float:hypothermia_damage = 1.0,
	damage_interval = 5000) {
	if(hypothermia_damage < 0.0 || damage_interval < 1000) {
		#if SW_DEBUG
		print("** [SmartWeather] Invalid survival damage or interval.");
		#endif
		return false;
	}

	SW_SurvivalActive = enabled;
	SW_HypothermiaDamage = hypothermia_damage;
	SW_HypothermiaInterval = damage_interval;
	return true;
}

// Clears active diseases and notifies the gamemode. (Limpa as doenças ativas e notifica o gamemode.)
stock sw_player_clear_diseases(playerid) {
	for(new diseaseid = 0; diseaseid < SW_MAX_DISEASES; diseaseid++) {
		if(!SW_PlayerDiseaseState[playerid][diseaseid]) {
			continue;
		}

		SW_PlayerDiseaseState[playerid][diseaseid] = false;
		CallLocalFunction("OnPlayerDiseaseChange", "iii",
			playerid, diseaseid, 0);
	}
	return true;
}

stock SW_Diseases(bool:enabled = false) {
	SW_DiseasesActive = enabled;
	if(!enabled) {
		for(new playerid = 0; playerid < MAX_PLAYERS; playerid++) {
			sw_player_clear_diseases(playerid);
			SW_PlayerDiseasesEnabled[playerid] = false;
		}
	}
	return true;
}

stock SW_PlayerDiseases(playerid, bool:enabled = true) {
	if(!sw_player_valid(playerid)) {
		return false;
	}

	SW_PlayerDiseasesEnabled[playerid] = enabled;
	if(!enabled) {
		sw_player_clear_diseases(playerid);
	}
	return true;
}

stock SW_PlayerSetDisease(playerid, diseaseid, bool:active) {
	if(!SW_DiseasesActive || !sw_player_valid(playerid)
		|| diseaseid < 0 || diseaseid >= SW_MAX_DISEASES
		|| !SW_PlayerDiseasesEnabled[playerid]) {
		return false;
	}

	if(SW_PlayerDiseaseState[playerid][diseaseid] != _:active) {
		SW_PlayerDiseaseState[playerid][diseaseid] = _:active;
		CallLocalFunction("OnPlayerDiseaseChange", "iii",
			playerid, diseaseid, _:active);
	}
	return true;
}

stock SW_PlayerHasDisease(playerid, diseaseid) {
	if(!sw_player_valid(playerid) || diseaseid < 0
		|| diseaseid >= SW_MAX_DISEASES) {
		return false;
	}
	return SW_DiseasesActive && SW_PlayerDiseasesEnabled[playerid]
		&& SW_PlayerDiseaseState[playerid][diseaseid];
}

stock Float:SW_PlayerWetness(playerid) {
	if(!sw_player_valid(playerid)) {
		return -1.0;
	}
	return SW_PlayerWetnessValue[playerid];
}

stock Float:SW_PlayerTemperature(playerid) {
	if(!sw_player_valid(playerid)) {
		return -1.0;
	}
	return SW_PlayerBodyTemperatureValue[playerid];
}

stock SW_PlayerIsWet(playerid) {
	if(!sw_player_valid(playerid)) {
		return false;
	}
	return SW_PlayerWetnessValue[playerid] >= SW_PLAYER_WETNESS_THRESHOLD;
}

stock SW_PlayerSetSurvival(playerid, Float:wetness, Float:temperature) {
	if(!sw_player_valid(playerid)
		|| wetness < 0.0 || wetness > 100.0
		|| temperature < SW_PLAYER_MIN_BODY_TEMPERATURE
		|| temperature > SW_PLAYER_MAX_BODY_TEMPERATURE) {
		return false;
	}

	SW_PlayerWetnessValue[playerid] = wetness;
	SW_PlayerBodyTemperatureValue[playerid] = temperature;
	SW_PlayerSurvivalTick[playerid] = GetTickCount();
	return true;
}

// Weather and zone queries (Consultas de clima e zonas)
stock SW_Wind(&Float:windx, &Float:windy, &Float:strength) {
	windx = SW_WindX;
	windy = SW_WindY;
	strength = SW_WindStrength;
	return true;
}

stock SW_ZoneWind(zoneid, &Float:windx, &Float:windy, &Float:strength) {
	if(zoneid < 0 || zoneid >= SW_MAX_ZONES || !SW_ZoneActive[zoneid]
		|| SW_ZoneFading[zoneid]) {
		return false;
	}
	windx = SW_ZoneWindX[zoneid];
	windy = SW_ZoneWindY[zoneid];
	strength = SW_ZoneWindStrength[zoneid];
	return true;
}

// Zone visuals and merging (Visualização e mesclagem de zonas)
stock SW_ZoneShow(zoneid, color, squares = 12) {
	if(zoneid < 0 || zoneid >= SW_MAX_ZONES || !SW_ZoneActive[zoneid]
		|| SW_ZoneFading[zoneid]) {
		return false;
	}

	if(squares < 1) {
		return false;
	}

	if(SW_ZoneMergeEnabled[zoneid] || SW_ZoneMerged[zoneid]) {
		SW_ZoneShapeColor[zoneid] = SW_ZoneMergeColor[zoneid];
	} else {
		SW_ZoneShapeColor[zoneid] = color;
	}
	SW_ZoneShapeSquares[zoneid] = squares;
	return sw_zone_draw(zoneid);
}

// Set custom_color to override the severity palette.
// Example: SW_ZoneMerge(zoneid, 600.0, 0.0, 0xAA00FF66, SW_ZONE_RED, true);
// (Exemplo: SW_ZoneMerge(zoneid, 600.0, 0.0, 0xAA00FF66, SW_ZONE_RED, true);)
stock SW_ZoneMerge(zoneid, Float:max_radius = 600.0, Float:map_limit = 0.0,
	color = SW_COLOR_ZONE_BLUE, severity = -1, bool:custom_color = false) {
	if(zoneid < 0 || zoneid >= SW_MAX_ZONES || !SW_ZoneActive[zoneid]
		|| SW_ZoneFading[zoneid]) {
		return false;
	}

	if(severity < -1 || severity > SW_ZONE_RED) {
		return false;
	}

	if(max_radius <= 0.0 || max_radius < SW_ZoneRadius[zoneid] || map_limit < 0.0
		|| (map_limit > 0.0 && (max_radius >= map_limit
			|| SW_ZoneRadius[zoneid] >= map_limit))) {
		#if SW_DEBUG
		print("** [SmartWeather] Invalid merge radius or map limit.");
		#endif
		return false;
	}

	if(!SW_ZoneMergeEnabled[zoneid]) {
		if(severity != -1) {
			SW_ZoneSeverityLevel[zoneid] = severity;
		} else {
			new roll = random(100);
			if(roll < SW_RED_ZONE_CHANCE) {
				SW_ZoneSeverityLevel[zoneid] = SW_ZONE_RED;
			} else if(random(100) < 70) {
				SW_ZoneSeverityLevel[zoneid] = SW_ZONE_LIGHT_BLUE;
			} else {
				SW_ZoneSeverityLevel[zoneid] = SW_ZONE_DARK_BLUE;
			}
		}

		if(SW_ZoneSeverityLevel[zoneid] == SW_ZONE_RED) {
			SW_ZoneWeather[zoneid] = SW_WEATHER_STORM;
			SW_ZonePriority[zoneid] += 1000;
			SW_ZoneWetRate[zoneid] = 3.5;
			SW_ZoneTemperatureRate[zoneid] = -0.5;
			SW_ZoneWindStrength[zoneid] = 1.0;
			if(SW_ZoneLabel[zoneid][0] == '\0')
				format(SW_ZoneLabel[zoneid], SW_ZONE_LABEL_LENGTH, "tempestade severa");
			SW_ZoneSpeedFactor[zoneid] = 0.35;
		} else {
			if(sw_storm(SW_ZoneWeather[zoneid])) {
				SW_ZoneWeather[zoneid] = SW_WEATHER_RAIN;
			}
			SW_ZoneWetRate[zoneid] = 1.0;
		}
		switch(SW_ZoneSeverityLevel[zoneid]) {
			case SW_ZONE_LIGHT_BLUE: {
				SW_ZoneSpeedFactor[zoneid] *= 0.65;
			}
		}
		sw_zone_random_size(zoneid);
		if(SW_ZoneSeverityLevel[zoneid] != SW_ZONE_RED) {
			sw_zone_random_speed(zoneid, GetTickCount());
		}
	}

	new bool:use_custom_color =
		(custom_color || color != SW_COLOR_ZONE_BLUE);
	SW_ZoneCustomMergeColor[zoneid] = use_custom_color;
	if(use_custom_color) {
		SW_ZoneMergeColor[zoneid] = color;
	} else {
		SW_ZoneMergeColor[zoneid] =
			sw_zone_default_merge_color(SW_ZoneSeverityLevel[zoneid]);
	}

	SW_ZoneMergeEnabled[zoneid] = true;
	SW_ZoneMergeMaxRadius[zoneid] = max_radius;
	SW_ZoneMergeMapLimit[zoneid] = map_limit;
	SW_ZoneShapeColor[zoneid] = SW_ZoneMergeColor[zoneid];
	SW_ZoneRadius[zoneid] = SW_ZoneRequestedRadius[zoneid]
		* SW_ZoneSizeFactor[zoneid];
	if(SW_ZoneRadius[zoneid] > SW_ZoneMergeMaxRadius[zoneid])
		SW_ZoneRadius[zoneid] = SW_ZoneMergeMaxRadius[zoneid];
	if(SW_ZoneMergeMapLimit[zoneid] > 0.0
		&& SW_ZoneRadius[zoneid] >= SW_ZoneMergeMapLimit[zoneid]) {
		SW_ZoneRadius[zoneid] = SW_ZoneMergeMapLimit[zoneid] - 1.0;
	}
	SW_ZoneTargetRadius[zoneid] = SW_ZoneRadius[zoneid];
	sw_zone_merge_clamp_target(zoneid);

	return sw_zone_merge_overlaps(zoneid);
}

stock sw_zone_default_merge_color(severity) {
	switch(severity) {
		case SW_ZONE_LIGHT_BLUE: return SW_COLOR_ZONE_LIGHT_BLUE;
		case SW_ZONE_DARK_BLUE: return SW_COLOR_ZONE_DARK_BLUE;
		case SW_ZONE_RED: return SW_COLOR_ZONE_RED;
	}
	return SW_COLOR_ZONE_BLUE;
}

stock SW_ZoneSeverity(zoneid) {
	if(zoneid < 0 || zoneid >= SW_MAX_ZONES || !SW_ZoneActive[zoneid]
		|| SW_ZoneFading[zoneid]) {
		return -1;
	}
	return SW_ZoneSeverityLevel[zoneid];
}

// Zone movement and removal (Movimentação e remoção de zonas)
stock sw_zone_random_speed(zoneid, tick) {
	SW_ZoneSpeedFactor[zoneid] = 0.35 + (float(random(106)) / 100.0);
	SW_ZoneSpeedChangeTick[zoneid] = tick + 20000 + random(40001);
	return true;
}

stock sw_zone_random_size(zoneid) {
	switch(SW_ZoneSeverityLevel[zoneid]) {
		case SW_ZONE_LIGHT_BLUE: SW_ZoneSizeFactor[zoneid] = 0.65 + (float(random(26)) / 100.0);
		case SW_ZONE_DARK_BLUE: SW_ZoneSizeFactor[zoneid] = 0.95 + (float(random(31)) / 100.0);
		case SW_ZONE_RED: SW_ZoneSizeFactor[zoneid] = 1.40 + (float(random(46)) / 100.0);
	}
	return true;
}

stock SW_ZoneMove(zoneid, Float:x, Float:y, Float:radius, weatherid = -1, color = -1) {
	if(zoneid < 0 || zoneid >= SW_MAX_ZONES || !SW_ZoneActive[zoneid]
		|| SW_ZoneFading[zoneid]) {
		return false;
	}

	if(radius <= 0.0
		|| (weatherid != -1
			&& (weatherid < 0 || weatherid > SW_MAX_WEATHER_ID))) {
		return false;
	}
	new Float:effective_radius = radius;
	if(SW_ZoneMergeEnabled[zoneid]) {
		effective_radius *= SW_ZoneSizeFactor[zoneid];
	}
	if(SW_ZoneMergeEnabled[zoneid]
		&& effective_radius > SW_ZoneMergeMaxRadius[zoneid]) {
		#if SW_DEBUG
		print("** [SmartWeather] Zone radius clamped to its merge limit.");
		#endif
		effective_radius = SW_ZoneMergeMaxRadius[zoneid];
	}
	if(SW_ZoneMergeEnabled[zoneid] && SW_ZoneMergeMapLimit[zoneid] > 0.0
		&& effective_radius >= SW_ZoneMergeMapLimit[zoneid]) {
		#if SW_DEBUG
		print("** [SmartWeather] Zone radius clamped to its map limit.");
		#endif
		effective_radius = SW_ZoneMergeMapLimit[zoneid] - 1.0;
	}

	SW_ZoneRequestedRadius[zoneid] = radius;
	SW_ZoneTargetRadius[zoneid] = effective_radius;
	if(SW_ZoneMergeEnabled[zoneid]) {
		SW_ZoneTargetX[zoneid] = x;
		SW_ZoneTargetY[zoneid] = y;
		sw_zone_merge_clamp_target(zoneid);
	} else {
		SW_ZoneX[zoneid] = x;
		SW_ZoneY[zoneid] = y;
		SW_ZoneTargetX[zoneid] = x;
		SW_ZoneTargetY[zoneid] = y;
		SW_ZoneRadius[zoneid] = effective_radius;
	}
	if(weatherid != -1) {
		if(SW_ZoneSeverityLevel[zoneid] == SW_ZONE_RED) {
			SW_ZoneWeather[zoneid] = SW_WEATHER_STORM;
		} else if(SW_ZoneMergeEnabled[zoneid] && sw_storm(weatherid)) {
			SW_ZoneWeather[zoneid] = SW_WEATHER_RAIN;
		} else {
			SW_ZoneWeather[zoneid] = weatherid;
		}
	}
	if(color != -1 && SW_ZoneShapeCount[zoneid] > 0
		&& !SW_ZoneMerged[zoneid] && !SW_ZoneMergeEnabled[zoneid]) {
		SW_ZoneShapeColor[zoneid] = color;
	}

	if(SW_ZoneMergeEnabled[zoneid]) {
		return sw_zone_merge_overlaps(zoneid, false);
	}

	if(SW_ZoneShapeCount[zoneid] > 0) {
		return sw_zone_draw(zoneid);
	}
	return true;
}

stock SW_ZoneRemove(zoneid) {
	if(zoneid < 0 || zoneid >= SW_MAX_ZONES || !SW_ZoneActive[zoneid]) {
		return false;
	}

	if(SW_ZoneFading[zoneid]) {
		return true;
	}

	return sw_zone_begin_fade(zoneid);
}

stock SW_ZonesClear() {
	SW_ZoneCount = 0;
	for(new zoneid = 0; zoneid < SW_MAX_ZONES; zoneid++) {
		sw_zone_shape_destroy(zoneid);
		SW_ZoneActive[zoneid] = false;
		SW_ZoneFading[zoneid] = false;
		SW_ZoneAutoCreated[zoneid] = false;
		SW_ZoneMergeEnabled[zoneid] = false;
		SW_ZoneMerged[zoneid] = false;
	}
	return true;
}

// Zone fade handling (Controle do desaparecimento das zonas)
stock sw_zone_begin_fade(zoneid) {
	if(SW_ZoneFading[zoneid]) {
		return true;
	}

	if(SW_ZoneShapeCount[zoneid] == 0
		|| SW_ZONE_FADE_DURATION <= 0) {
		sw_zone_shape_destroy(zoneid);
		SW_ZoneActive[zoneid] = false;
		SW_ZoneMergeEnabled[zoneid] = false;
		SW_ZoneMerged[zoneid] = false;
		SW_ZoneCount--;
		return true;
	}

	SW_ZoneFading[zoneid] = true;
	SW_ZoneFadeStartTick[zoneid] = GetTickCount();
	SW_ZoneFadeVisualTick[zoneid] = SW_ZoneFadeStartTick[zoneid];
	SW_ZoneFadeDuration[zoneid] = sw_zone_fade_duration(zoneid);
	SW_ZoneFadeStartRadius[zoneid] = SW_ZoneRadius[zoneid];
	SW_ZoneTargetX[zoneid] = SW_ZoneX[zoneid];
	SW_ZoneTargetY[zoneid] = SW_ZoneY[zoneid];
	SW_ZoneMergeEnabled[zoneid] = false;
	SW_ZoneCount--;
	return true;
}

stock sw_zone_fade_duration(zoneid) {
	new minimum = SW_ZONE_FADE_MIN_DURATION;
	new maximum = SW_ZONE_FADE_DURATION;
	if(SW_ZoneSeverityLevel[zoneid] == SW_ZONE_RED) {
		minimum = SW_RED_ZONE_FADE_MIN_DURATION;
		maximum = SW_RED_ZONE_FADE_MAX_DURATION;
	}

	if(minimum < 1) {
		minimum = 1;
	}
	if(maximum < minimum) {
		maximum = minimum;
	}

	new duration = minimum + random(maximum - minimum + 1);
	if(SW_ZoneSeverityLevel[zoneid] == SW_ZONE_RED
		&& SW_ZoneSizeFactor[zoneid] > 1.40) {
		duration += floatround((SW_ZoneSizeFactor[zoneid] - 1.40) * 120000.0);
	}
	return duration;
}

stock sw_zone_fade_update(tick) {
	for(new zoneid = 0; zoneid < SW_MAX_ZONES; zoneid++) {
		if(!SW_ZoneActive[zoneid] || !SW_ZoneFading[zoneid]) {
			continue;
		}

		new elapsed = tick - SW_ZoneFadeStartTick[zoneid];
		if(elapsed >= SW_ZoneFadeDuration[zoneid]) {
			sw_zone_shape_destroy(zoneid);
			SW_ZoneActive[zoneid] = false;
			SW_ZoneFading[zoneid] = false;
			SW_ZoneMerged[zoneid] = false;
			continue;
		}

		if(tick - SW_ZoneFadeVisualTick[zoneid] < SW_ZONE_FADE_INTERVAL) {
			continue;
		}

		new Float:progress = float(elapsed) / float(SW_ZoneFadeDuration[zoneid]);
		SW_ZoneRadius[zoneid] = SW_ZoneFadeStartRadius[zoneid] * (1.0 - progress);
		if(SW_ZoneRadius[zoneid] < 2.0) {
			SW_ZoneRadius[zoneid] = 2.0;
		}
		if(!sw_zone_draw(zoneid)) {
			#if SW_DEBUG
			printf("** [SmartWeather] Could not update fading zone %d.", zoneid);
			#endif
		}
		SW_ZoneFadeVisualTick[zoneid] = tick;
	}
	return true;
}

// Player connection state (Estado de conexão dos jogadores)
stock sw_player_reset_state(playerid) {
	SW_PlayerWeatherKnown[playerid] = false;
	SW_PlayerZoneKnown[playerid] = false;
	SW_PlayerGlobalKnown[playerid] = false;
	SW_PlayerClockKnown[playerid] = false;
	SW_PlayerWetnessValue[playerid] = 0.0;
	SW_PlayerBodyTemperatureValue[playerid] = 37.0;
	SW_PlayerDiseasesEnabled[playerid] = false;

	for(new diseaseid = 0; diseaseid < SW_MAX_DISEASES; diseaseid++)
		SW_PlayerDiseaseState[playerid][diseaseid] = false;
	return true;
}

hook OnPlayerConnect(playerid) {
	sw_player_reset_state(playerid);
	new tick = GetTickCount();
	SW_PlayerSurvivalTick[playerid] = tick;
	SW_PlayerDamageTick[playerid] = tick;

	for(new zoneid = 0; zoneid < SW_MAX_ZONES; zoneid++) {
		if(SW_ZoneActive[zoneid] && SW_ZoneShapeCount[zoneid] > 0) {
			sw_zone_shape_show_for_player(playerid, zoneid);
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason) {
	sw_player_reset_state(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

// Player weather and grip (Clima e aderência do jogador)
stock Float:SW_Grip(weatherid) {
	switch(weatherid) {
		case 8, 13: return 0.85;
		case 9, 14: return 0.88;
		case 16, 19: return 0.70;
	}
	return 1.0;
}

stock Float:SW_PlayerGrip(playerid) {
	if(!sw_player_valid(playerid)) {
		return 1.0;
	}

	new Float:grip = SW_Grip(sw_player_weather[playerid]);
	new zoneid = SW_PlayerZone[playerid];
	if(zoneid < 0 || zoneid >= SW_MAX_ZONES || !SW_ZoneActive[zoneid]) {
		return grip;
	}

	if(SW_ZoneSeverityLevel[zoneid] == SW_ZONE_RED) {
		grip *= 0.45;
	} else {
		grip *= 0.90;
	}
	return grip;
}

stock SW_PlayerWeather(playerid) {
	if(!sw_player_valid(playerid)) {
		return -1;
	}

	if(!SW_PlayerWeatherKnown[playerid]) {
		return SW_BaseWeather;
	}
	return sw_player_weather[playerid];
}

// Main weather and survival update loop (Loop principal de atualização do clima e sobrevivência)
public SW_Update() {
	new tick = GetTickCount();
	sw_tick(tick);
	sw_zone_move(tick);
	sw_zone_fade_update(tick);
	sw_auto_zone_update(tick);
	if(tick - SW_NextWindChange >= 0) {
		sw_wind_change();
	}

	for(new playerid = 0; playerid < MAX_PLAYERS; playerid++) {
		if(!IsPlayerConnected(playerid)) {
			SW_PlayerWeatherKnown[playerid] = false;
			SW_PlayerClockKnown[playerid] = false;
			SW_PlayerZoneKnown[playerid] = false;
			SW_PlayerGlobalKnown[playerid] = false;
			continue;
		}

		if(!SW_PlayerClockKnown[playerid]) {
			SetPlayerTime(playerid, SW_ClockMinute / 60, SW_ClockMinute % 60);
			SW_PlayerClockKnown[playerid] = true;
		}

		new zoneid = -1;
		new local_weather = sw_area(playerid, zoneid);
		new target_weather = sw_blend_weather(SW_BaseWeather, local_weather);
		new old_zoneid = SW_PlayerZoneKnown[playerid] ? SW_PlayerZone[playerid] : -1;
		new old_local_weather = SW_PlayerZoneKnown[playerid]
			? SW_PlayerLocalWeather[playerid] : -1;
		new bool:zone_changed = zoneid != old_zoneid;
		new bool:global_changed = !SW_PlayerGlobalKnown[playerid]
			|| SW_PlayerGlobalWeather[playerid] != SW_BaseWeather;

		if(zone_changed) {
			if(old_zoneid != -1) {
				CallLocalFunction("OnPlayerLeaveWeatherZone", "iiii",
					playerid, old_zoneid, SW_ZoneTypeId[old_zoneid], old_local_weather);
			}
			if(zoneid != -1) {
				CallLocalFunction("OnPlayerEnterWeatherZone", "iiii",
					playerid, zoneid, SW_ZoneTypeId[zoneid], local_weather);
			}
		}

		SW_PlayerZone[playerid] = zoneid;
		SW_PlayerLocalWeather[playerid] = local_weather;
		SW_PlayerZoneKnown[playerid] = true;
		SW_PlayerGlobalWeather[playerid] = SW_BaseWeather;
		SW_PlayerGlobalKnown[playerid] = true;
		if(!SW_PlayerWeatherKnown[playerid]) {
			SW_PlayerWeatherKnown[playerid] = true;
			sw_player_weather[playerid] = target_weather;
			SW_PlayerTargetWeather[playerid] = target_weather;
			SetPlayerWeather(playerid, target_weather);
			sw_notify(playerid, -1, target_weather);
			CallLocalFunction("OnPlayerWeatherBlend", "iiii",
				playerid, SW_BaseWeather, local_weather, target_weather);

			if(SW_SurvivalActive) {
				SW_PlayerWetnessValue[playerid] = 0.0;
				SW_PlayerBodyTemperatureValue[playerid] = 37.0;
				SW_PlayerSurvivalTick[playerid] = tick;
				SW_PlayerDamageTick[playerid] = tick;
			}
		} else if(target_weather != SW_PlayerTargetWeather[playerid] || zone_changed) {
			SW_PlayerTargetWeather[playerid] = target_weather;
			CallLocalFunction("OnPlayerWeatherBlend", "iiii",
				playerid, SW_BaseWeather, local_weather, target_weather);
			if(zoneid != -1 || old_zoneid != -1) {
				sw_set(playerid, target_weather);
			} else {
				sw_fade(playerid, tick);
			}
		} else if(global_changed) {
			CallLocalFunction("OnPlayerWeatherBlend", "iiii",
				playerid, SW_BaseWeather, local_weather, target_weather);
		}
		if(sw_player_weather[playerid] != SW_PlayerTargetWeather[playerid]
			&& tick - SW_PlayerNextStep[playerid] >= 0) {
			sw_step(playerid, tick);
		}

		if(SW_SurvivalActive) {
			sw_survival_update(playerid, zoneid, target_weather, tick);
		}
	}
	return true;
}

stock sw_auto_zone_update(tick) {
	if(!SW_AutoZonesEnabled || tick - SW_AutoZoneNextSpawn < 0) {
		return true;
	}

	new active_auto_zones;
	for(new zoneid = 0; zoneid < SW_MAX_ZONES; zoneid++) {
		if(SW_ZoneActive[zoneid] && !SW_ZoneFading[zoneid]
			&& SW_ZoneAutoCreated[zoneid]) {
			active_auto_zones++;
		}
	}

	if(active_auto_zones < SW_AutoZoneMaxActive) {
		new Float:position_x = SW_AutoZoneMinX
			+ ((SW_AutoZoneMaxX - SW_AutoZoneMinX)
				* (float(random(10001)) / 10000.0));
		new Float:position_y = SW_AutoZoneMinY
			+ ((SW_AutoZoneMaxY - SW_AutoZoneMinY)
				* (float(random(10001)) / 10000.0));
		new Float:radius = SW_AutoZoneMinRadius
			+ ((SW_AutoZoneMaxRadius - SW_AutoZoneMinRadius)
				* (float(random(10001)) / 10000.0));
		new zoneid = SW_Zone(position_x, position_y, radius,
			SW_WEATHER_STORM);

		if(zoneid != SW_INVALID_ZONE_ID) {
			SW_ZoneAutoCreated[zoneid] = true;
			SW_ZoneType(zoneid, SW_ZONE_RED, "tempestade severa");
			SW_ZoneSurvival(zoneid, 3.5, -0.5);
			if(!SW_ZoneMerge(zoneid, SW_AutoZoneMaxRadius, 0.0,
				SW_COLOR_ZONE_RED, SW_ZONE_RED)
				|| !SW_ZoneShow(zoneid, SW_COLOR_ZONE_RED)) {
				#if SW_DEBUG
				printf("** [SmartWeather] Could not create automatic storm zone %d.", zoneid);
				#endif
				SW_ZoneRemove(zoneid);
			}
		}
	}

	SW_AutoZoneNextSpawn = tick + SW_AutoZoneInterval;
	return true;
}

// Clock and local weather selection (Relógio e seleção do clima local)
stock sw_tick(tick) {
	new elapsed = tick - SW_LastClockUpdate;
	SW_LastClockUpdate = tick;

	if(SW_ClockScale > 0.0 && elapsed > 0) {
		SW_ClockRemainder += (float(elapsed) * SW_ClockScale) / 60000.0;

		new advanced_minutes = floatround(SW_ClockRemainder, floatround_floor);
		if(advanced_minutes > 0) {
			SW_ClockMinute = (SW_ClockMinute + advanced_minutes) % 1440;
			SW_ClockRemainder -= float(advanced_minutes);
		}
	}

	sw_time();
	return true;
}

stock sw_time() {
	if(SW_LastAppliedClockMinute == SW_ClockMinute) {
		return true;
	}

	new hour = SW_ClockMinute / 60;
	new minute = SW_ClockMinute % 60;

	for(new playerid = 0; playerid < MAX_PLAYERS; playerid++) {
		if(!IsPlayerConnected(playerid)) {
			continue;
		}

		SetPlayerTime(playerid, hour, minute);
		SW_PlayerClockKnown[playerid] = true;
	}

	SW_LastAppliedClockMinute = SW_ClockMinute;
	return true;
}

stock sw_area(playerid, &selected_zone) {
	new weather_id = -1;
	new selected_priority;
	new bool:selected = false;
	selected_zone = -1;
	new Float:player_x, Float:player_y, Float:player_z;
	GetPlayerPos(playerid, player_x, player_y, player_z);

	for(new zone_id = 0; zone_id < SW_MAX_ZONES; zone_id++) {
		if(!SW_ZoneActive[zone_id] || SW_ZoneFading[zone_id]) {
			continue;
		}

		new Float:delta_x = player_x - SW_ZoneX[zone_id];
		new Float:delta_y = player_y - SW_ZoneY[zone_id];
		new Float:radius = SW_ZoneRadius[zone_id];

		if((delta_x * delta_x) + (delta_y * delta_y) <= radius * radius
			&& (!selected || SW_ZonePriority[zone_id] > selected_priority)) {
			weather_id = SW_ZoneWeather[zone_id];
			selected_priority = SW_ZonePriority[zone_id];
			selected_zone = zone_id;
			selected = true;
		}
	}
	return weather_id;
}

// Weather classification and survival calculations (Classificação do clima e cálculos de sobrevivência)
stock sw_blend_weather(globalweather, localweather) {
	if(localweather == -1) {
		return globalweather;
	}

	new global_stage = sw_stage(globalweather);
	new local_stage = sw_stage(localweather);
	return (local_stage >= global_stage) ? localweather : globalweather;
}

stock sw_weather_name(weatherid, output[], output_size) {
	switch(weatherid) {
		case SW_WEATHER_DRY: format(output, output_size, "tempo limpo");
		case SW_WEATHER_CLOUDY: format(output, output_size, "ceu nublado");
		case SW_WEATHER_RAIN, 13: format(output, output_size, "chuva");
		case SW_WEATHER_STORM, 19: format(output, output_size, "tempestade");
		case 4, 20: format(output, output_size, "nevoeiro");
		case 6: format(output, output_size, "nublado");
		default: format(output, output_size, "clima adverso");
	}
	return true;
}

stock sw_weather_is_wet(weatherid) {
	return (weatherid == SW_WEATHER_RAIN || weatherid == SW_WEATHER_STORM
		|| weatherid == 13 || weatherid == 19 || weatherid == 21);
}

stock Float:sw_ambient_temperature(weatherid) {
	new Float:temperature = 22.0;
	switch(sw_season) {
		case SW_SEASON_SUMMER: temperature = 30.0;
		case SW_SEASON_AUTUMN: temperature = 18.0;
		case SW_SEASON_WINTER: temperature = 8.0;
	}

	if(SW_ClockMinute < 360 || SW_ClockMinute >= 1200) {
		temperature -= 4.0;
	}
	if(weatherid == SW_WEATHER_STORM || weatherid == 19) {
		temperature -= 3.0;
	} else if(sw_weather_is_wet(weatherid)) {
		temperature -= 1.5;
	}
	return temperature;
}

stock sw_survival_update(playerid, zoneid, weatherid, tick) {
	new elapsed = tick - SW_PlayerSurvivalTick[playerid];
	if(elapsed <= 0) {
		return true;
	}
	SW_PlayerSurvivalTick[playerid] = tick;

	new Float:seconds = float(elapsed) / 1000.0;
	new Float:wetness_rate =
		(sw_weather_is_wet(SW_BaseWeather) || sw_weather_is_wet(weatherid)) ? 2.0 : -0.5;
	new Float:temperature_rate;
	new Float:wind_strength = SW_WindStrength;

	if(zoneid != -1) {
		wetness_rate += SW_ZoneWetRate[zoneid];
		temperature_rate += SW_ZoneTemperatureRate[zoneid];
		wind_strength = SW_ZoneWindStrength[zoneid];
	}

	SW_PlayerWetnessValue[playerid] += wetness_rate * seconds;
	if(SW_PlayerWetnessValue[playerid] < 0.0) {
		SW_PlayerWetnessValue[playerid] = 0.0;
	} else if(SW_PlayerWetnessValue[playerid] > 100.0) {
		SW_PlayerWetnessValue[playerid] = 100.0;
	}

	new Float:ambient = sw_ambient_temperature(weatherid);
	new Float:exposure = 0.0002 + (SW_PlayerWetnessValue[playerid] * 0.000004)
		+ (wind_strength * 0.00015);
	SW_PlayerBodyTemperatureValue[playerid] +=
		((ambient - SW_PlayerBodyTemperatureValue[playerid]) * exposure
		+ temperature_rate) * seconds;
	if(SW_PlayerBodyTemperatureValue[playerid] < SW_PLAYER_MIN_BODY_TEMPERATURE) {
		SW_PlayerBodyTemperatureValue[playerid] = SW_PLAYER_MIN_BODY_TEMPERATURE;
	} else if(SW_PlayerBodyTemperatureValue[playerid] > SW_PLAYER_MAX_BODY_TEMPERATURE) {
		SW_PlayerBodyTemperatureValue[playerid] = SW_PLAYER_MAX_BODY_TEMPERATURE;
	}

	new bool:is_wet =
		(SW_PlayerWetnessValue[playerid] >= SW_PLAYER_WETNESS_THRESHOLD);
	if(SW_DiseasesActive && SW_PlayerDiseasesEnabled[playerid]) {
		SW_PlayerSetDisease(playerid, SW_DISEASE_FLU,
			is_wet && SW_PlayerBodyTemperatureValue[playerid] < 36.0);
		SW_PlayerSetDisease(playerid, SW_DISEASE_HYPOTHERMIA,
			is_wet && SW_PlayerBodyTemperatureValue[playerid] < 35.0);
		SW_PlayerSetDisease(playerid, SW_DISEASE_HEATSTROKE,
			SW_PlayerBodyTemperatureValue[playerid] >= 39.0);
	}
	CallLocalFunction("OnPlayerSurvivalUpdate", "iffi", playerid,
		SW_PlayerWetnessValue[playerid], SW_PlayerBodyTemperatureValue[playerid], _:is_wet);

	if(SW_PlayerBodyTemperatureValue[playerid] < 35.0
		&& (is_wet || (ambient < 10.0 && SW_PlayerBodyTemperatureValue[playerid] < 34.0))
		&& tick - SW_PlayerDamageTick[playerid] >= SW_HypothermiaInterval) {
		SW_PlayerDamageTick[playerid] = tick;
		if(SW_HypothermiaDamage > 0.0) {
			new Float:health;
			GetPlayerHealth(playerid, health);
			if(health > 0.0) {
				health -= SW_HypothermiaDamage;
				if(health < 0.0) {
					health = 0.0;
				}
				SetPlayerHealth(playerid, health);
			}
		}
	}
	return true;
}

stock sw_zone_move(tick) {
	for(new zoneid = 0; zoneid < SW_MAX_ZONES; zoneid++) {
		if(!SW_ZoneActive[zoneid] || SW_ZoneFading[zoneid]
			|| !SW_ZoneMergeEnabled[zoneid]) {
			continue;
		}

		new elapsed = tick - SW_ZoneMoveTick[zoneid];
		SW_ZoneMoveTick[zoneid] = tick;
		if(elapsed <= 0) {
			continue;
		}
		if(tick - SW_ZoneSpeedChangeTick[zoneid] >= 0) {
			sw_zone_random_speed(zoneid, tick);
		}

		new bool:changed;
		new Float:delta_x = SW_ZoneTargetX[zoneid] - SW_ZoneX[zoneid];
		new Float:delta_y = SW_ZoneTargetY[zoneid] - SW_ZoneY[zoneid];
		new Float:distance = floatsqroot((delta_x * delta_x) + (delta_y * delta_y));
		new Float:max_step = SW_ZONE_MOVE_SPEED * SW_ZoneSpeedFactor[zoneid]
			* (float(elapsed) / 1000.0);
		if(distance > 0.0 && distance <= max_step) {
			SW_ZoneX[zoneid] = SW_ZoneTargetX[zoneid];
			SW_ZoneY[zoneid] = SW_ZoneTargetY[zoneid];
			changed = true;
		} else if(distance > max_step) {
			new Float:scale = max_step / distance;
			SW_ZoneX[zoneid] += delta_x * scale;
			SW_ZoneY[zoneid] += delta_y * scale;
			changed = true;
		}

		new Float:radius_delta = SW_ZoneTargetRadius[zoneid] - SW_ZoneRadius[zoneid];
		new Float:max_radius_step = SW_ZONE_RADIUS_SPEED * (float(elapsed) / 1000.0);
		if(radius_delta > max_radius_step) {
			SW_ZoneRadius[zoneid] += max_radius_step;
			changed = true;
		} else if(radius_delta < -max_radius_step) {
			SW_ZoneRadius[zoneid] -= max_radius_step;
			changed = true;
		} else if(radius_delta != 0.0) {
			SW_ZoneRadius[zoneid] = SW_ZoneTargetRadius[zoneid];
			changed = true;
		}

		if(changed) {
			sw_zone_merge_overlaps(zoneid, false);
			if(tick - SW_ZoneVisualTick[zoneid] >= SW_ZONE_VISUAL_INTERVAL) {
				if(SW_ZoneShapeCount[zoneid] > 0
					&& !sw_zone_draw(zoneid)) {
					#if SW_DEBUG
					printf("** [SmartWeather] Could not update moving zone %d.", zoneid);
					#endif
				}
				SW_ZoneVisualTick[zoneid] = tick;
			}
		}
	}
	return true;
}

// Draw each zone with its own stable, irregular outline. (Desenha cada zona com contorno irregular e estável.)
stock sw_zone_draw(zoneid) {
	sw_zone_shape_destroy(zoneid);

	new segment_count = SW_ZoneShapeSquares[zoneid];
	if(segment_count < 8) {
		segment_count = 8;
	} else if(segment_count > SW_ZONE_SHAPE_PARTS) {
		segment_count = SW_ZONE_SHAPE_PARTS;
	}

	new available_zones = GetGangZonesAvailable();
	if(segment_count > available_zones) {
		segment_count = available_zones;
	}
	if(segment_count <= 0) {
		#if SW_DEBUG
		printf("** [SmartWeather] Could not draw zone %d.", zoneid);
		#endif
		return false;
	}

	new Float:radius = SW_ZoneRadius[zoneid];
	new frequency_one = 2 + (SW_ZoneShapeSeed[zoneid] % 4);
	new frequency_two = 3 + ((SW_ZoneShapeSeed[zoneid] / 7) % 4);
	new frequency_three = 5 + ((SW_ZoneShapeSeed[zoneid] / 17) % 4);
	new Float:phase_one = float(SW_ZoneShapeSeed[zoneid]);
	new Float:phase_two = float((SW_ZoneShapeSeed[zoneid] * 7) % 360);
	new Float:phase_three = float((SW_ZoneShapeSeed[zoneid] * 13) % 360);

	for(new part = 0; part < segment_count; part++) {
		new Float:relative_y = -1.0
			+ (2.0 * (float(part) + 0.5) / float(segment_count));
		new Float:base_width = floatsqroot(1.0 - (relative_y * relative_y));
		new Float:shape_wave =
			(floatsin((relative_y * 180.0 * float(frequency_one)) + phase_one,
				degrees) * 0.55)
			+ (floatsin((relative_y * 180.0 * float(frequency_two)) + phase_two,
				degrees) * 0.30)
			+ (floatsin((relative_y * 180.0 * float(frequency_three)) + phase_three,
				degrees) * 0.15);
		new Float:width = radius * base_width * (1.0 + (shape_wave * 0.28));
		new Float:center_shift = radius * base_width
			* floatsin((relative_y * 180.0) + phase_two, degrees) * 0.10;
		new Float:min_y = SW_ZoneY[zoneid] - radius
			+ (2.0 * radius * float(part) / float(segment_count));
		new Float:max_y = SW_ZoneY[zoneid] - radius
			+ (2.0 * radius * float(part + 1) / float(segment_count));
		new gangzoneid = GangZoneCreate(
			SW_ZoneX[zoneid] + center_shift - width,
			min_y,
			SW_ZoneX[zoneid] + center_shift + width,
			max_y
		);

		if(gangzoneid == INVALID_GZ_SHAPE_ID) {
			sw_zone_shape_destroy(zoneid);
			#if SW_DEBUG
			printf("** [SmartWeather] Could not draw zone %d.", zoneid);
			#endif
			return false;
		}

		SW_ZoneShapeZones[zoneid][SW_ZoneShapeCount[zoneid]++] = gangzoneid;
		GangZoneShowForAll(gangzoneid, SW_ZoneShapeColor[zoneid]);
	}
	return true;
}

stock sw_zone_shape_destroy(zoneid) {
	for(new part = 0; part < SW_ZoneShapeCount[zoneid]; part++) {
		GangZoneDestroy(SW_ZoneShapeZones[zoneid][part]);
	}
	SW_ZoneShapeCount[zoneid] = 0;
	return true;
}

stock sw_zone_shape_show_for_player(playerid, zoneid) {
	for(new part = 0; part < SW_ZoneShapeCount[zoneid]; part++) {
		GangZoneShowForPlayer(playerid, SW_ZoneShapeZones[zoneid][part],
			SW_ZoneShapeColor[zoneid]);
	}
	return true;
}

stock sw_zone_merge_overlaps(zoneid, bool:redraw = true) {
	sw_zone_merge_clamp(zoneid);

	for(new otherid = 0; otherid < SW_MAX_ZONES; otherid++) {
		if(otherid == zoneid || !SW_ZoneActive[otherid] || SW_ZoneFading[otherid]
			|| !SW_ZoneMergeEnabled[otherid]) {
			continue;
		}

		new Float:delta_x = SW_ZoneX[otherid] - SW_ZoneX[zoneid];
		new Float:delta_y = SW_ZoneY[otherid] - SW_ZoneY[zoneid];
		new Float:combined_radius = SW_ZoneRadius[zoneid] + SW_ZoneRadius[otherid];
		if((delta_x * delta_x) + (delta_y * delta_y)
			> combined_radius * combined_radius) {
			continue;
		}

		new Float:zone_area = SW_ZoneRadius[zoneid] * SW_ZoneRadius[zoneid];
		new Float:other_area = SW_ZoneRadius[otherid] * SW_ZoneRadius[otherid];
		new Float:combined_area = zone_area + other_area;
		// Weight the merged center and rates by each zone's area. (O centro e os índices combinados usam a área de cada zona como peso.)
		new Float:other_weight = other_area / combined_area;

		SW_ZoneX[zoneid] += (SW_ZoneX[otherid] - SW_ZoneX[zoneid]) * other_weight;
		SW_ZoneY[zoneid] += (SW_ZoneY[otherid] - SW_ZoneY[zoneid]) * other_weight;
		SW_ZoneTargetX[zoneid] +=
			(SW_ZoneTargetX[otherid] - SW_ZoneTargetX[zoneid]) * other_weight;
		SW_ZoneTargetY[zoneid] +=
			(SW_ZoneTargetY[otherid] - SW_ZoneTargetY[zoneid]) * other_weight;
		SW_ZoneWetRate[zoneid] +=
			(SW_ZoneWetRate[otherid] - SW_ZoneWetRate[zoneid]) * other_weight;
		SW_ZoneTemperatureRate[zoneid] +=
			(SW_ZoneTemperatureRate[otherid] - SW_ZoneTemperatureRate[zoneid])
			* other_weight;
		SW_ZoneWindX[zoneid] +=
			(SW_ZoneWindX[otherid] - SW_ZoneWindX[zoneid]) * other_weight;
		SW_ZoneWindY[zoneid] +=
			(SW_ZoneWindY[otherid] - SW_ZoneWindY[zoneid]) * other_weight;
		SW_ZoneWindStrength[zoneid] +=
			(SW_ZoneWindStrength[otherid] - SW_ZoneWindStrength[zoneid])
			* other_weight;
		if(SW_ZoneMergeMaxRadius[otherid] < SW_ZoneMergeMaxRadius[zoneid]) {
			SW_ZoneMergeMaxRadius[zoneid] = SW_ZoneMergeMaxRadius[otherid];
		}
		if(SW_ZoneMergeMapLimit[otherid] > 0.0
			&& (SW_ZoneMergeMapLimit[zoneid] == 0.0
				|| SW_ZoneMergeMapLimit[otherid] < SW_ZoneMergeMapLimit[zoneid])) {
			SW_ZoneMergeMapLimit[zoneid] = SW_ZoneMergeMapLimit[otherid];
		}
		SW_ZoneRadius[zoneid] = floatsqroot(combined_area);
		if(SW_ZoneRadius[zoneid] > SW_ZoneMergeMaxRadius[zoneid]) {
			SW_ZoneRadius[zoneid] = SW_ZoneMergeMaxRadius[zoneid];
		}
		SW_ZoneTargetRadius[zoneid] = SW_ZoneRadius[zoneid];
		sw_zone_merge_clamp(zoneid);
		sw_zone_merge_clamp_target(zoneid);
		if(SW_ZonePriority[otherid] > SW_ZonePriority[zoneid]) {
			SW_ZoneWeather[zoneid] = SW_ZoneWeather[otherid];
			SW_ZonePriority[zoneid] = SW_ZonePriority[otherid];
			SW_ZoneTypeId[zoneid] = SW_ZoneTypeId[otherid];
			format(SW_ZoneLabel[zoneid], SW_ZONE_LABEL_LENGTH, "%s", SW_ZoneLabel[otherid]);
		}
		new old_severity = SW_ZoneSeverityLevel[zoneid];
		if(SW_ZoneSeverityLevel[otherid] > SW_ZoneSeverityLevel[zoneid]) {
			SW_ZoneSeverityLevel[zoneid] = SW_ZoneSeverityLevel[otherid];
		}
		if(SW_ZoneSeverityLevel[zoneid] != old_severity) {
			sw_zone_random_size(zoneid);
		}
		if(SW_ZoneSeverityLevel[zoneid] == SW_ZONE_RED) {
			SW_ZoneWeather[zoneid] = SW_WEATHER_STORM;
			SW_ZoneWetRate[zoneid] = 3.5;
			SW_ZoneTemperatureRate[zoneid] = -0.5;
			SW_ZoneWindStrength[zoneid] = 1.0;
			SW_ZoneSpeedFactor[zoneid] = 0.35;
			if(SW_ZoneLabel[zoneid][0] == '\0')
				format(SW_ZoneLabel[zoneid], SW_ZONE_LABEL_LENGTH, "tempestade severa");
		}

		SW_ZoneMerged[zoneid] = true;
		if(SW_ZoneSeverityLevel[zoneid] == SW_ZONE_RED
			&& !SW_ZoneCustomMergeColor[zoneid]) {
			SW_ZoneMergeColor[zoneid] = SW_COLOR_ZONE_RED;
		}
		SW_ZoneShapeColor[zoneid] = SW_ZoneMergeColor[zoneid];
		sw_zone_begin_fade(otherid);

		CallLocalFunction("OnWeatherZoneMerge", "iiff", zoneid, otherid,
			SW_ZoneX[zoneid], SW_ZoneY[zoneid]);
	}

	if(redraw && SW_ZoneShapeCount[zoneid] > 0) {
		return sw_zone_draw(zoneid);
	}
	return true;
}

stock sw_zone_merge_clamp(zoneid) {
	if(SW_ZoneMergeMapLimit[zoneid] <= 0.0) {
		return true;
	}

	new Float:max_center = SW_ZoneMergeMapLimit[zoneid] - SW_ZoneRadius[zoneid];
	new Float:distance = floatsqroot((SW_ZoneX[zoneid] * SW_ZoneX[zoneid])
		+ (SW_ZoneY[zoneid] * SW_ZoneY[zoneid]));
	if(distance > max_center) {
		new Float:scale = max_center / distance;
		SW_ZoneX[zoneid] *= scale;
		SW_ZoneY[zoneid] *= scale;
	}
	return true;
}

stock sw_zone_merge_clamp_target(zoneid) {
	if(SW_ZoneMergeMapLimit[zoneid] <= 0.0) {
		return true;
	}

	new Float:max_center = SW_ZoneMergeMapLimit[zoneid] - SW_ZoneTargetRadius[zoneid];
	new Float:distance = floatsqroot((SW_ZoneTargetX[zoneid] * SW_ZoneTargetX[zoneid])
		+ (SW_ZoneTargetY[zoneid] * SW_ZoneTargetY[zoneid]));
	if(distance > max_center) {
		new Float:scale = max_center / distance;
		SW_ZoneTargetX[zoneid] *= scale;
		SW_ZoneTargetY[zoneid] *= scale;
	}
	return true;
}

// Player weather transitions (Transições de clima dos jogadores)
stock sw_fade(playerid, tick) {
	new current_stage = sw_stage(sw_player_weather[playerid]);
	new target_stage = sw_stage(SW_PlayerTargetWeather[playerid]);
	new stage_distance = target_stage - current_stage;
	if(stage_distance < 0) {
		stage_distance = -stage_distance;
	}

	if(SW_TransitionSeconds == 0 || stage_distance == 0) {
		sw_set(playerid, SW_PlayerTargetWeather[playerid]);
		return true;
	}

	SW_PlayerStepInterval[playerid] = (SW_TransitionSeconds * 1000) / stage_distance;
	if(SW_PlayerStepInterval[playerid] < SW_UPDATE_INTERVAL) {
		SW_PlayerStepInterval[playerid] = SW_UPDATE_INTERVAL;
	}

	SW_PlayerNextStep[playerid] = tick + SW_PlayerStepInterval[playerid];
	return true;
}

stock sw_step(playerid, tick) {
	new current_stage = sw_stage(sw_player_weather[playerid]);
	new target_stage = sw_stage(SW_PlayerTargetWeather[playerid]);

	if(current_stage < target_stage) {
		current_stage++;
	} else if(current_stage > target_stage) {
		current_stage--;
	}

	if(current_stage == target_stage) {
		sw_set(playerid, SW_PlayerTargetWeather[playerid]);
		return true;
	}

	switch(current_stage) {
		case 0: sw_set(playerid, SW_WEATHER_DRY);
		case 1: sw_set(playerid, SW_WEATHER_CLOUDY);
		case 2: sw_set(playerid, SW_WEATHER_RAIN);
		case 3: sw_set(playerid, SW_WEATHER_STORM);
	}

	SW_PlayerNextStep[playerid] = tick + SW_PlayerStepInterval[playerid];
	return true;
}

stock sw_set(playerid, weatherid) {
	new old_weather = sw_player_weather[playerid];
	sw_player_weather[playerid] = weatherid;
	SetPlayerWeather(playerid, weatherid);
	sw_notify(playerid, old_weather, weatherid);

	return true;
}

stock sw_notify(playerid, old_weather, new_weather) {
	CallLocalFunction(
		"OnPlayerWeatherChange",
		"iiif",
		playerid,
		old_weather,
		new_weather,
		SW_PlayerGrip(playerid)
	);
	return true;
}

stock sw_stage(weatherid) {
	switch(weatherid) {
		case 8, 13: return 2;
		case 16, 19: return 3;
		case 6, 7, 9, 12, 14, 15, 17, 18: return 1;
	}
	return 0;
}

stock sw_storm(weatherid) {
	return (weatherid == 16 || weatherid == 19);
}

// Backwards-compatible names (Nomes mantidos para compatibilidade)
#define SW_SetWeather			SW_Weather
#define SW_SetSeason				SW_Season
#define SW_SetClock				SW_Clock
#define SW_AddZone				SW_Zone
#define SW_ShowZone				SW_ZoneShow
#define SW_MoveZone				SW_ZoneMove
#define SW_RemoveZone				SW_ZoneRemove
#define SW_ClearZones				SW_ZonesClear
#define SW_GetGripFactor			SW_Grip
#define SW_GetPlayerWeather			SW_PlayerWeather
