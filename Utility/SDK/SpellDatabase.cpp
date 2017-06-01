#include "SpellDatabase.hpp"

static std::vector<SpellInfo> g_vecPredefinedSpellDatabase = {
	SpellInfo("Ahri", "AhriOrbofDeception", "AhriOrbMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 925.0f, 100.0f, 1750.0f, { }, { }),
	SpellInfo("Ahri", "AhriSeduce", "AhriSeduceMissile", kSlotE, kSpellSkillshotLine, 0.25f, 1000.0f, 60.0f, 1550.0f, { }, { }),
	SpellInfo("Amumu", "BandageToss", "SadMummyBandageToss", kSlotQ, kSpellSkillshotLine, 0.25f, 1100.0f, 80.0f, 2000.0f, { }, { }),
	SpellInfo("Anivia", "FlashFrostSpell", "FlashFrostSpell", kSlotQ, kSpellSkillshotLine, 0.25f, 1250.0f, 110.0f, 850.0f, { }, { }),
	SpellInfo("Ashe", "EnchantedCrystalArrow", "EnchantedCrystalArrow", kSlotR, kSpellSkillshotLine, 0.25f, 15000.0f, 130.0f, 1600.0f, { }, { }),
	SpellInfo("Bard", "BardQ", "BardQMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 950.0f, 60.0f, 1600.0f, { }, { }),
	SpellInfo("Bard", "BardR", "BardR", kSlotR, kSpellSkillshotCircle, 0.25f, 3400.0f, 350.0f, 2100.0f, { }, { }),
	SpellInfo("Blitzcrank", "RocketGrab", "RocketGrabMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 1050.0f, 70.0f, 1800.0f, { "RocketGrabMissile" }, { }),
	SpellInfo("Brand", "BrandQ", "BrandQMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 1100.0f, 60.0f, 2000.0f, { }, { }),
	SpellInfo("Brand", "BrandW", "", kSlotW, kSpellSkillshotCircle, 0.85f, 1100.0f, 250.0f, 0.0f, { }, { }),
	SpellInfo("Braum", "BraumQ", "BraumQMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 1000.0f, 100.0f, 1200.0f, { }, { }),
	SpellInfo("Braum", "BraumRWrapper", "BraumRMissile", kSlotR, kSpellSkillshotLine, 0.50f, 1250.0f, 115.0f, 1125.0f, { }, { }),
	SpellInfo("Caitlyn", "CaitlynPiltoverPeacemaker", "CaitlynPiltoverPeacemaker", kSlotQ, kSpellSkillshotLine, 0.625f, 1300.0f, 90.0f, 2200.0f, { }, { }),
	SpellInfo("Caitlyn", "CaitlynEntrapment", "CaitlynEntrapmentMissile", kSlotE, kSpellSkillshotLine, 0.125f, 950.0f, 80.0f, 2000.0f, { }, { }),
	SpellInfo("Cassiopeia", "CassiopeiaQ", "CassiopeiaQ", kSlotQ, kSpellSkillshotCircle, 0.75f, 850.0f, 200.0f, 0.0f, { }, { }),
	SpellInfo("Chogath", "Rupture", "Rupture", kSlotQ, kSpellSkillshotCircle, 1.2f, 950.0f, 250.0f, 0.0f, { }, { }),
	SpellInfo("Corki", "PhosphorusBomb", "PhosphorusBombMissile", kSlotQ, kSpellSkillshotCircle, 0.5f, 825.0f, 270.0f, 1125.0f, { }, { }),
	SpellInfo("Corki", "MissileBarrage", "MissileBarrageMissile", kSlotR, kSpellSkillshotLine, 0.175f, 1300.0f, 40.0f, 2000.0f, { }, { }),
	SpellInfo("Diana", "DianaArc", "DianaArc", kSlotQ, kSpellSkillshotCircle, 0.25f, 850.0f, 150.0f, 1400.0f, { }, { }),
	SpellInfo("Draven", "DravenRCast", "DravenR", kSlotR, kSpellSkillshotLine, 0.25f, 15000.0f, 160.0f, 2000.0f, { }, { }),
	SpellInfo("Ekko", "EkkoQ", "EkkoQMis", kSlotQ, kSpellSkillshotLine, 0.25f, 950.0f, 60.0f, 1650.0f, { }, { }),
	SpellInfo("Ekko", "EkkoW", "EkkoW", kSlotW, kSpellSkillshotCircle, 3.750f, 1750.0f, 375.0f, 1650.0f, { }, { }),
	SpellInfo("Elise", "EliseHumanE", "EliseHumanE", kSlotE, kSpellSkillshotLine, 0.25f, 1100.0f, 55.0f, 1600.0f, { }, { }),
	SpellInfo("Ezreal", "EzrealMysticShot", "EzrealMysticShotMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 1200.0f, 60.0f, 2000.0f, { "EzrealMysticShotWrapper" }, { "EzrealMysticShotPulseMissile" }),
	SpellInfo("Ezreal", "EzrealEssenceFlux", "EzrealEssenceFluxMissile", kSlotW, kSpellSkillshotLine, 0.25f, 1050.0f, 80.0f, 1600.0f, { }, { }),
	SpellInfo("Ezreal", "EzrealTrueshotBarrage", "EzrealTrueshotBarrage", kSlotR, kSpellSkillshotLine, 1.0f, 15000.0f, 160.0f, 2000.0f, { }, { }),
	SpellInfo("Fizz", "FizzR", "FizzRMissile", kSlotR, kSpellSkillshotLine, 0.25f, 1300.0f, 120.0f, 1350.0f, { }, { }),
	SpellInfo("Galio", "GalioRighteousGust", "GalioRighteousGust", kSlotE, kSpellSkillshotLine, 0.25f, 1280.0f, 120.0f, 1300.0f, { }, { }),
	SpellInfo("Galio", "GalioResoluteSmite", "GalioResoluteSmite", kSlotQ, kSpellSkillshotCircle, 0.25f, 1040.0f, 235.0f, 1200.0f, { }, { }),
	SpellInfo("Gragas", "GragasQ", "GragasQMissile", kSlotQ, kSpellSkillshotCircle, 0.5f, 975.0f, 260.0f, 1000.0f, { }, { }),
	SpellInfo("Gragas", "GragasR", "GragasR", kSlotR, kSpellSkillshotCircle, 0.25f, 1050.0f, 350.0f, 1750.0f, { }, { }),
	SpellInfo("Heimerdinger", "HeimerdingerW", "HeimerdingerWAttack2", kSlotW, kSpellSkillshotLine, 0.25f, 1350.0f, 70.0f, 1800.0f, { }, { "HeimerdingerWAttack2Ult" }),
	SpellInfo("Heimerdinger", "HeimerdingerE", "heimerdingerespell", kSlotE, kSpellSkillshotCircle, 0.325f, 925.0f, 150.0f, 1200.0f, { }, { "heimerdingerespell_ult", "heimerdingerespell_ult2", "heimerdingerespell_ult3" }),
	SpellInfo("Illaoi", "IllaoiQ", "IllaoiQ", kSlotQ, kSpellSkillshotLine, 0.75f, 850.0f, 100.0f, 0.0f, { }, { }),
	SpellInfo("Illaoi", "IllaoiE", "IllaoiEMis", kSlotE, kSpellSkillshotLine, 0.25f, 950.0f, 50.0f, 1900.0f, { }, { }),
	SpellInfo("Ivern", "IvernQ", "IvernQ", kSlotQ, kSpellSkillshotLine, 0.25f, 1150.0f, 65.0f, 1300.0f, { }, { }),
	SpellInfo("Janna", "HowlingGale", "HowlingGaleSpell", kSlotQ, kSpellSkillshotLine, 0.25f, 1700.0f, 120.0f, 900.0f, { }, { }),
	SpellInfo("JarvanIV", "JarvanIVDragonStrike", "JarvanIVDragonStrike", kSlotQ, kSpellSkillshotLine, 0.25f, 845.0f, 80.0f, 2000.0f, { }, { }),
	SpellInfo("Jayce", "jayceshockblastmis", "jayceshockblastmis", kSlotQ, kSpellSkillshotLine, 0.25f, 1170.0f, 70.0f, 1450.0f, { }, { }),
	SpellInfo("Jayce",  "jayceshockblastwallmis", "jayceshockblastwallmis", kSlotQ, kSpellSkillshotLine, 0.25f, 1600.0f, 70.0f, 2350.0f, { }, { }),
	SpellInfo("Jinx", "JinxW", "JinxWMissile", kSlotW, kSpellSkillshotLine, 0.6f, 1500.0f, 60.0f, 3300.0f, { "JinxWMissile" }, { }),
	SpellInfo("Jinx", "JinxR", "JinxR", kSlotR, kSpellSkillshotLine, 0.6f, 15000.0f, 140.0f, 1700.0f, { "JinxRWrapper" }, { "JinxRWrapper", "JinxRMissile" }),
	SpellInfo("Jhin", "JhinW", "JhinWMissile", kSlotW, kSpellSkillshotLine, 0.65f, 2550.0f, 40.0f, 0.0f, { }, { }),
	SpellInfo("Jhin", "JhinRShot", "JhinRShotMis", kSlotR, kSpellSkillshotLine, 0.25f, 3500.0f, 80.0f, 5000.0f, { }, { "JhinRShotMis4" }),
	SpellInfo("Karma", "KarmaQ", "KarmaQMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 1050.0f, 60.0f, 1700.0f, { "KarmaQMissileMantra" }, { "KarmaQMissileMantra" }),
	SpellInfo("Karthus", "KarthusLayWasteA1", "KarthusLayWasteA1", kSlotQ, kSpellSkillshotCircle, 0.625f, 875.0f, 190.0f, 0.0f, { }, { }),
	SpellInfo("Kennen", "KennenShurikenHurlMissile1", "KennenShurikenHurlMissile1", kSlotQ, kSpellSkillshotLine, 0.125f, 1050.0f, 50.0f, 1700.0f, { }, { }),
	SpellInfo("KhaZix", "KhazixE", "khazixe", kSlotE, kSpellSkillshotCircle, 0.1f, 700.0f, 300.0f, 1200.0f, { }, { }),
	SpellInfo("KhaZix", "khazixelong", "khazixelong", kSlotE, kSpellSkillshotCircle, 0.1f, 900.0f, 300.0f, 1200.0f, { }, { }),
	SpellInfo("KogMaw", "KogMawQ", "KogMawQ", kSlotQ, kSpellSkillshotLine, 0.25f, 1200.0f, 70.0f, 1650.0f, { }, { }),
	SpellInfo("KogMaw", "KogMawLivingArtillery", "KogMawLivingArtillery", kSlotR, kSpellSkillshotCircle, 1.1f, 2200.0f, 235.0f, 0.0f, { }, { }),
	SpellInfo("Leblanc", "LeblancE", "LeblancEMissile", kSlotE, kSpellSkillshotLine, 0.25f, 960.0f, 55.0f, 1750.0f, { "LeblancRE" }, { "LeblancRE" }),
	SpellInfo("LeeSin", "BlindMonkQOne", "BlindMonkQOne", kSlotQ, kSpellSkillshotLine, 0.25f, 1100.0f, 60.0f, 1800.0f, { }, { }),
	SpellInfo("Leona", "LeonaSolarFlare", "LeonaSolarFlare", kSlotR, kSpellSkillshotCircle, 0.625f, 1200.0f, 250.0f, 0.0f, { }, { }),
	SpellInfo("Leona", "LeonaZenithBlade", "LeonaZenithBladeMissile", kSlotE, kSpellSkillshotLine, 0.2f, 900.0f, 70.0f, 2000.0f, { }, { }),
	SpellInfo("Lissandra", "LissandraQ", "LissandraQMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 825.0f, 75.0f, 2200.0f, { }, { }),
	SpellInfo("Lulu", "LuluQ", "LuluQMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 925.0f, 60.0f, 1450.0f, { }, { }),
	SpellInfo("Lux",  "LuxLightStrikeKugel", "LuxLightStrikeKugel", kSlotE, kSpellSkillshotCircle, 0.25f, 1100.0f, 330.0f, 1300.0f, { }, { }),
	SpellInfo("Lux", "LuxLightBinding", "LuxLightBindingMis", kSlotQ, kSpellSkillshotLine, 0.25f, 1300.0f, 70.0f, 1200.0f, { }, { }),
	SpellInfo("Lux", "LuxMaliceCannon", "LuxMaliceCannon", kSlotR, kSpellSkillshotLine, 1.0f, 3300.0f, 190.0f, 0.0f, { }, { }),
	SpellInfo("Morgana", "DarkBindingMissile", "DarkBindingMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 1300.0f, 80.0f, 1200.0f, { }, { }),
	SpellInfo("Nami", "NamiQ", "NamiQMissile", kSlotQ, kSpellSkillshotCircle, 0.45f, 875.0f, 180.0f, 2500.0f, { }, { }),
	SpellInfo("Nami", "NamiR", "NamiRMissile", kSlotR, kSpellSkillshotLine, 0.5f, 2750.0f, 250.0f, 850.0f, { }, { }),
	SpellInfo("Nautilus", "NautilusAnchorDrag", "NautilusAnchorDragMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 1150.0f, 90.0f, 2000.0f, { }, { }),
	SpellInfo("Nidalee", "JavelinToss", "JavelinToss", kSlotQ, kSpellSkillshotLine, 0.25f, 1500.0f, 40.0f, 1300.0f, { }, { }),
	SpellInfo("Olaf", "OlafAxeThrowCast", "olafaxethrow", kSlotQ, kSpellSkillshotLine, 0.25f, 1000.0f, 90.0f, 1600.0f, { }, { }),
	SpellInfo("Quinn", "QuinnQ", "QuinnQ", kSlotQ, kSpellSkillshotLine, 0.25f, 1050.0f, 60.0f, 1550.0f, { }, { }),
	SpellInfo("Rengar", "RengarE", "RengarEMis", kSlotE, kSpellSkillshotLine, 0.25f, 1000.0f, 70.0f, 1500.0f, { "RengarEEmp" }, { "RengarEEmpMis" }),
	SpellInfo("Rumble", "RumbleGrenade", "RumbleGrenadeMissile", kSlotE, kSpellSkillshotLine, 0.25f, 950.0f, 60.0f, 2000.0f, { }, { }),
	SpellInfo("Ryze", "RyzeQ", "RyzeQ", kSlotQ, kSpellSkillshotLine, 0.25f, 900.0f, 60.0f, 1700.0f, { }, { }),
	SpellInfo("Sejuani", "SejuaniGlacialPrisonCast", "SejuaniGlacialPrison", kSlotR, kSpellSkillshotLine, 0.25f, 1200.0f, 110.0f, 1600.0f, { }, { }),
	SpellInfo("Sivir", "SivirQ", "SivirQMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 1275.0f, 100.0f, 1350.0f, { }, { }),
	SpellInfo("Sona", "SonaR", "SonaR", kSlotR, kSpellSkillshotLine, 0.25f, 1000.0f, 150.0f, 2400.0f, { }, { }),
	SpellInfo("Soraka", "SorakaQ", "SorakaQMissile", kSlotQ, kSpellSkillshotCircle, 0.25f, 970.0f, 230.0f, 1100.0f, { }, { }),
	SpellInfo("Syndra", "SyndraQ", "SyndraQSpell", kSlotQ, kSpellSkillshotCircle, 0.6f, 800.0f, 210.0f, 0.0f, { }, { }),
	SpellInfo("Syndra", "syndrawcast", "syndrawcast", kSlotW, kSpellSkillshotCircle, 0.25f, 950.0f, 210.0f, 0.0f, { }, { }),
	SpellInfo("TahmKench", "TahmKenchQ", "TahmKenchQMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 800.0f, 70.0f, 2000.0f, { }, { }),
	SpellInfo("Thresh", "ThreshQ", "ThreshQMissile", kSlotQ, kSpellSkillshotLine, 0.5f, 1200.0f, 70.0f, 1900.0f, { }, { }),
	SpellInfo("TwistedFate", "WildCards", "SealFateMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 1450.0f, 40.0f, 1000.0f, { }, { }),
	SpellInfo("Varus", "VarusQ", "VarusQMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 1525.0f, 75.0f, 1900.0f, { }, { }),
	SpellInfo("Veigar", "VeigarBalefulStrike", "VeigarBalefulStrikeMis", kSlotQ, kSpellSkillshotLine, 0.25f, 950.0f, 70.0f, 2200.0f, { }, { }),
	SpellInfo("Velkoz", "VelkozE", "VelkozE", kSlotE, kSpellSkillshotCircle, 0.25f, 800.0f, 225.0f, 0.0f, { }, { }),
	SpellInfo("Velkoz", "VelkozQ", "VelkozQMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 1100.0f, 50.0f, 2100.0f, { "VelkozQSplit" }, { "VelkozQMissileSplit" }),
	SpellInfo("Viktor", "ViktorDeathRay", "ViktorDeathRayMissile", kSlotE, kSpellSkillshotLine, 0.25f, 815.f, 75.0f, 1050.0f, { }, { "ViktorEAugMissile" }),
	SpellInfo("Xerath", "XerathMageSpear", "XerathMageSpearMissile", kSlotE, kSpellSkillshotLine, 0.2f, 1125.0f, 60.0f, 1600.0f, { }, { }),
	SpellInfo("Xerath", "XerathLocusOfPower2", "xerathrmissilewrapper", kSlotR, kSpellSkillshotCircle, 0.6f, 5600.0f, 200.0f, 0.0f, { "XerathLocusPulse" }, { }),
	SpellInfo("Yasuo", "YasuoQ3", "YasuoQ3Mis", kSlotQ, kSpellSkillshotLine, 0.3f, 1150.0f, 90.0f, 1250.0f, { }, { }),
	SpellInfo("Yasuo", "YasuoQ", "YasuoQ", kSlotQ, kSpellSkillshotLine, 0.4f, 550.0f, 40.0f, 0.0f, { "YasuoQ2" }, { "YasuoQ2" }),
	SpellInfo("Zed", "ZedQ", "ZedQMissile", kSlotQ, kSpellSkillshotLine, 0.25f, 925.0f, 50.0f, 1700.0f, { }, { }),
	SpellInfo("Ziggs", "ZiggsQ", "ZiggsQ", kSlotQ, kSpellSkillshotCircle, 0.125f, 850.0f, 150.0f, 1700.0f, { }, { "ZiggsQSpell2", "ZiggsQSpell3" }),
	SpellInfo("Ziggs", "ZiggsW", "ZiggsW", kSlotW, kSpellSkillshotCircle, 0.25f, 1000.0f, 275.0f, 2000.0f, { }, { }),
	SpellInfo("Ziggs", "ZiggsE", "ZiggsE", kSlotE, kSpellSkillshotCircle, 0.25f, 2000.0f, 235.0f, 3000.0f, { }, { }),
	SpellInfo("Ziggs", "ZiggsR", "ZiggsR", kSlotR, kSpellSkillshotCircle, 0.4f, 5300.0f, 500.0f, 0.0f, { }, { }),
	SpellInfo("Zilean", "ZileanQ", "ZileanQMissile", kSlotQ, kSpellSkillshotCircle, 0.65f, 900.0f, 150.0f, 0.0f, { }, { }),
	SpellInfo("Zyra", "ZyraE", "ZyraEMissile", kSlotE, kSpellSkillshotLine, 0.25f, 1150.0f, 70.0f, 1400.0f, { }, { })
};

std::vector<SpellInfo> g_vecSpellDatabase;

static std::array<DashInfo, 3> g_arrDashDatabase{
	DashInfo("SummonerFlash", 425.0f),
	DashInfo("EzrealArcaneShift", 475.0f),
	DashInfo("Deceive", 400.0f)
};

static std::array<SmiteInfo, 3> g_arrSmiteDatabase{
	SmiteInfo("Chogath", kSlotR, kTargetCast, 325.0f, 0.25f),
	SmiteInfo("Khazix", kSlotQ, kTargetCast, 325.0f, 0.25f),
	SmiteInfo("Pantheon", kSlotQ, kTargetCast, 600.0f, 0.25f)
};

auto SpellDb::GetSpell(const char* champName, const char* spellName, eSpellSlot slot, SpellInfo& out) -> bool
{
	for (const auto& spell : g_vecSpellDatabase)
	{
		if (spell.m_slot == slot && !strcmp(spell.m_strChampionName, champName) && 
			!_stricmp(spell.m_strSpellName, spellName))
		{
			out = spell;
			return true;
		}
		else if (!spell.m_vecExtraSpellNames.empty())
		{
			auto extraSpellNames = spell.m_vecExtraSpellNames;
			if (std::find_if(extraSpellNames.begin(), extraSpellNames.end(), [&](const char* str)
			{
				return !_stricmp(str, spellName);
			}) != extraSpellNames.end())
			{
				out = spell;
				return true;
			}
		}
	}

	return false;
}

auto SpellDb::GetSpellByMissile(const char* projectileName, SpellInfo& out) -> bool
{
	for (const auto& spell : g_vecSpellDatabase)
	{
		if (!_stricmp(spell.m_strProjectileName, projectileName))
		{
			out = spell;
			return true;
		}
		else if (!spell.m_vecExtraProjectileNames.empty())
		{
			auto extraProjectileNames = spell.m_vecExtraProjectileNames;
			if (std::find_if(extraProjectileNames.begin(), extraProjectileNames.end(), [&](const char* str)
			{
				return !_stricmp(str, projectileName);

			}) != extraProjectileNames.end())
			{
				out = spell;
				return true;
			}
		}
	}

	return false;
}

auto SpellDb::GetDash(const char* spellName, DashInfo& out) -> bool
{
	auto i = std::find_if(g_arrDashDatabase.begin(), g_arrDashDatabase.end(), [&](const DashInfo& dash)
	{
		return !strcmp(dash.m_strSpellName, spellName);
	});

	if (i != g_arrDashDatabase.end())
	{
		out = *i;
		return true;
	}

	return false;
}

auto SpellDb::GetSmiteSpell(const char* championName, SmiteInfo& out) -> bool
{
	auto i = std::find_if(g_arrSmiteDatabase.begin(), g_arrSmiteDatabase.end(), [&](const SmiteInfo& smite)
	{
		return !strcmp(smite.m_strChampionName, championName);
	});

	if (i != g_arrSmiteDatabase.end())
	{
		out = *i;
		return true;
	}

	return false;
}

auto SpellDb::LoadSpells() -> void
{
	g_vecSpellDatabase.reserve(25);

	for (auto hero : GEntityList->GetAllHeros(false, true))
	{
		for (const auto& spell : g_vecPredefinedSpellDatabase)
		{
			if (!strcmp(spell.m_strChampionName, hero->ChampionName()))
				g_vecSpellDatabase.emplace_back(spell);
		}
	}
}