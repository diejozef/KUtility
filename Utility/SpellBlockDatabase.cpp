#include "SpellBlockDatabase.hpp"

SpellBlockDatabase::SpellBlockDatabase()
{
	m_mapSpellDatabase.emplace(std::make_pair("CamilleR", SpellBlockInfo("Camille", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("DariusExecute", SpellBlockInfo("Darius", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("Feast", SpellBlockInfo("Chogath", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("Headbutt", SpellBlockInfo("Alistar", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("IreliaEquilibriumStrike", SpellBlockInfo("Irelia", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("JarvanIVCataclysm", SpellBlockInfo("JarvanIV", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("MalzaharR", SpellBlockInfo("Malzahar", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("VeigarPrimordialBurst", SpellBlockInfo("Veigar", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("LissandraR", SpellBlockInfo("Lissandra", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("PoppyE", SpellBlockInfo("Poppy", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("Fling", SpellBlockInfo("Singed", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("WarwickQ", SpellBlockInfo("Warwick", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("GarenR", SpellBlockInfo("Garen", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("PantheonW", SpellBlockInfo("Pantheon", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("SyndraR", SpellBlockInfo("Syndra", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("BlindingDart", SpellBlockInfo("Teemo", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("VolibearW", SpellBlockInfo("Volibear", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("VayneCondemn", SpellBlockInfo("Vayne", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("PuncturingTaunt", SpellBlockInfo("Rammus", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("BlindMonkRKick", SpellBlockInfo("LeeSin", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("OlafRecklessStrike", SpellBlockInfo("Olaf", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("MaokaiUnstableGrowth", SpellBlockInfo("Maokai", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("SkarnerImpale", SpellBlockInfo("Skarner", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("TwoShivPoison", SpellBlockInfo("Shaco", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("Disintegrate", SpellBlockInfo("Annie", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("BrandR", SpellBlockInfo("Brand", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("LuluW", SpellBlockInfo("Lulu", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("RyzeW", SpellBlockInfo("Ryze", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("TalonNoxianDiplomacy", SpellBlockInfo("Talon", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("FizzQ", SpellBlockInfo("Fizz", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("GangplankQWrapper", SpellBlockInfo("Gangplank", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("PantheonQ", SpellBlockInfo("Pantheon", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("LucianQ", SpellBlockInfo("Lucian", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("VladimirQ", SpellBlockInfo("Vladimir", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("SeismicShard", SpellBlockInfo("Malphite", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("SwainTorment", SpellBlockInfo("Swain", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("Terrify", SpellBlockInfo("FiddleSticks", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("MalzaharE", SpellBlockInfo("Malzahar", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("JudicatorReckoning", SpellBlockInfo("Kayle", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("NullLance", SpellBlockInfo("Kassadin", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("IceBlast", SpellBlockInfo("Nunu", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("TahmKenchW", SpellBlockInfo("TahmKench", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("QuinnE", SpellBlockInfo("Quinn", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("JaxLeapStrike", SpellBlockInfo("Jax", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("KhazixQ", SpellBlockInfo("Khazix", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("EliseHumanQ", SpellBlockInfo("Elise", kSlotQ)));

	// Jayce spells with dummies(in case plugin gets loaded when he's in gun stance)
	m_mapSpellDatabase.emplace(std::make_pair("JayceToTheSkies", SpellBlockInfo("Jayce", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("JayceShockBlast", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("JayceThunderingBlow", SpellBlockInfo("Jayce", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("JayceAccelerationGate", SpellBlockInfo()));

	// AutoAttack buffs(and dummies)
	m_mapSpellDatabase.emplace(std::make_pair("PowerFistAttack", SpellBlockInfo("Blitzcrank", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("PowerFist", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("HecarimRampAttack", SpellBlockInfo("Hecarim", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("HecarimRamp", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("LeonaShieldOfDaybreakAttack", SpellBlockInfo("Leona", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("LeonaShieldOfDaybreak", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("NasusQAttack", SpellBlockInfo("Nasus", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("LeonaShieldOfDaybreak", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("RenektonExecute", SpellBlockInfo("Renekton", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("RenektonSuperExecute", SpellBlockInfo("Renekton", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("RenektonPreExecute", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("UdyrBearAttack", SpellBlockInfo("Udyr", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("UdyrBearAttackUlt", SpellBlockInfo("Udyr", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("UdyrBearStance", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("VolibearQAttack", SpellBlockInfo("Volibear", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("VolibearQ", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("XenZhaoThrust3", SpellBlockInfo("XinZhao", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("XenZhaoComboTarget", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("yorickqattack", SpellBlockInfo("Yorick", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("YorickQ", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("GarenQAttack", SpellBlockInfo("Garen", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("GarenQ", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("goldcardpreattack", SpellBlockInfo("TwistedFate", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("PickACard", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("DariusNoxianTacticsONHAttack", SpellBlockInfo("Darius", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("DariusNoxianTacticsONH", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("EkkoEAttack", SpellBlockInfo("Ekko", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("EkkoE", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("MonkeyKingQAttack", SpellBlockInfo("MonkeyKing", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("MonkeyKingDoubleAttack", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("viktorqbuff", SpellBlockInfo("Viktor", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("ViktorPowerTransfer", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("mordekaiserqattack2", SpellBlockInfo("Mordekaiser", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("MordekaiserMaceOfSpades", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("NidaleeTakedownAttack", SpellBlockInfo("Nidalee", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("Takedown", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("ShyvanaDoubleAttackHit", SpellBlockInfo("Shyvana", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("ShyvanaDoubleAttack", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("NautilusRavageStrikeAttack", SpellBlockInfo("Nautilus", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("NautilusPiercingGaze", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("ViEAttack", SpellBlockInfo("Vi", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("ViE", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("illaoiwattack", SpellBlockInfo("Illaoi", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("IllaoiW", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("MasochismAttack", SpellBlockInfo("DrMundo", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("Masochism", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("FizzWBasicAttack", SpellBlockInfo("Fizz", kSlotW)));
	m_mapSpellDatabase.emplace(std::make_pair("FizzW", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("JaxRelentlessAttack", SpellBlockInfo("Jax", kSlotR)));
	m_mapSpellDatabase.emplace(std::make_pair("JaxRelentlessAssault", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("ShenQAttack", SpellBlockInfo("Shen", kSlotQ)));
	m_mapSpellDatabase.emplace(std::make_pair("ShenQ", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("MasterYiDoubleStrike", SpellBlockInfo("MasterYi", kSlotE)));
	m_mapSpellDatabase.emplace(std::make_pair("WujuStyle", SpellBlockInfo()));

	// Missile spells(and dummies)
	m_mapSpellDatabase.emplace(std::make_pair("YasuoQ3Mis", SpellBlockInfo("Yasuo", kSlotQ, 210.0f)));
	m_mapSpellDatabase.emplace(std::make_pair("YasuoQW", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("IllaoiEMis", SpellBlockInfo("Illaoi", kSlotE, 200.0f)));
	m_mapSpellDatabase.emplace(std::make_pair("IllaoiE", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("RocketGrabMissile", SpellBlockInfo("Blitzcrank", kSlotQ, 220.0f)));
	m_mapSpellDatabase.emplace(std::make_pair("RocketGrab", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("ThreshQMissile", SpellBlockInfo("Thresh", kSlotQ, 200.0f)));
	m_mapSpellDatabase.emplace(std::make_pair("ThreshQ", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("DarkBindingMissile", SpellBlockInfo("Morgana", kSlotQ, 220.0f)));
	m_mapSpellDatabase.emplace(std::make_pair("EliseHumanE", SpellBlockInfo("Elise", kSlotE, 210.0f)));
	m_mapSpellDatabase.emplace(std::make_pair("PoppyRMissile", SpellBlockInfo("Poppy", kSlotR, 210.0f)));
	m_mapSpellDatabase.emplace(std::make_pair("PoppyR", SpellBlockInfo()));
	m_mapSpellDatabase.emplace(std::make_pair("RumbleGrenadeMissile", SpellBlockInfo("Rumble", kSlotE, 210.0f)));
	m_mapSpellDatabase.emplace(std::make_pair("RumbleGrenade", SpellBlockInfo()));

	// AoE spells
	m_mapAoESpells.emplace(std::make_pair("UFSlash", AoESpellBlockInfo(kSlotR, 270.0f, 1000.0f))); // Malphite R
	m_mapAoESpells.emplace(std::make_pair("InfernalGuardian", AoESpellBlockInfo(kSlotR, 250.0f, 600.0f))); // Annie R
	m_mapAoESpells.emplace(std::make_pair("DianaVortex", AoESpellBlockInfo(kSlotE, 450.0f, 0.0f, true))); // Diana E
	m_mapAoESpells.emplace(std::make_pair("NamiQ", AoESpellBlockInfo(kSlotQ, 140.0f, 875.0f))); // Nami Q
	m_mapAoESpells.emplace(std::make_pair("AatroxQ", AoESpellBlockInfo(kSlotQ, 220.0f, 650.0f))); // Aatrox Q
	m_mapAoESpells.emplace(std::make_pair("CurseoftheSadMummy", AoESpellBlockInfo(kSlotR, 500.0f, 0.0f, true))); // Amumu R
	m_mapAoESpells.emplace(std::make_pair("ShenE", AoESpellBlockInfo(kSlotE, 170.0f, 600.0f))); // Shen E
	m_mapAoESpells.emplace(std::make_pair("GnarBigW", AoESpellBlockInfo(kSlotW, 180.0f, 600.0f))); // MegaGnar W
	m_mapAoESpells.emplace(std::make_pair("GnarW", AoESpellBlockInfo())); // MegaGnar W Dummy
	m_mapAoESpells.emplace(std::make_pair("GnarR", AoESpellBlockInfo(kSlotR, 250.0f, 475.0f))); // Gnar R
	m_mapAoESpells.emplace(std::make_pair("GragasE", AoESpellBlockInfo(kSlotE, 200.0f, 600.0f))); // Gragas E
	m_mapAoESpells.emplace(std::make_pair("GragasR", AoESpellBlockInfo(kSlotR, 350.0f, 1000.0f))); // Gragas R
	m_mapAoESpells.emplace(std::make_pair("DariusAxeGrabCone", AoESpellBlockInfo(kSlotE, 130.0f, 500.0f))); // Darius E
	m_mapAoESpells.emplace(std::make_pair("MaokaiTrunkLine", AoESpellBlockInfo(kSlotQ, 150.0f, 0.0f, true))); // Maokai close Q
	m_mapAoESpells.emplace(std::make_pair("PoppyRSpellInstant", AoESpellBlockInfo(kSlotR, 100.0f, 300.0f))); // Poppy R instant cast
	m_mapAoESpells.emplace(std::make_pair("GalioE", AoESpellBlockInfo(kSlotE, 200.0f, 600.0f))); // Galio E
}

SpellBlockDatabase::~SpellBlockDatabase()
{

}

auto SpellBlockDatabase::IsBlockableSpell(const char* spellName) -> bool
{
	return m_mapSpellDatabase.find(std::string(spellName)) != m_mapSpellDatabase.end();
}

auto SpellBlockDatabase::GetBlockableSpell(const char* spellName) -> SpellBlockInfo
{
	return m_mapSpellDatabase[std::string(spellName)];
}

auto SpellBlockDatabase::IsBlockableAoESpell(const char* spellName) -> bool
{
	return m_mapAoESpells.find(std::string(spellName)) != m_mapAoESpells.end();
}

auto SpellBlockDatabase::GetBlockableAoESpell(const char* spellName) -> AoESpellBlockInfo
{
	return m_mapAoESpells[std::string(spellName)];
}
