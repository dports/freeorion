# AI can not currently add new building types to production queue
# AI can not currently design or build custom ships
# individual techs are to be removed from unusable list as AI programming progresses

outpostTechName =         "CON_ENV_ENCAPSUL"
exobotTechName =         "PRO_EXOBOTS"
gasOrbitalTechName = "PRO_ORBITAL_GEN"
solarOrbitalTechName = "PRO_SOL_ORB_GEN"

SupplyTechs=[ "CON_ORBITAL_CON" ,   "CON_CONTGRAV_ARCH",  "CON_GAL_INFRA",  "CON_ARCH_MONOFILS"]

cCamps=["CON_METRO_INFRA",   "CON_ARCH_PSYCH",  "CON_CONC_CAMP"]

def unusableTechsList():
    return []

def defenseTechs1():
    defTechs1=[
        "DEF_DEFENSE_NET_1", 
        "DEF_GARRISON_1", 
        ]
    return defTechs1

def defenseTechs2():
    defTechs2=[
        ]
    return defTechs2

def primaryMetaTechsList(index=0):
    "primary techs for all categories"
    #index=1-index
    #index=1
    if index == 0:
        primaryMetaTechs = [
            "SHP_WEAPON_1_2",
            "GRO_PLANET_ECOL",
            "SHP_DOMESTIC_MONSTER",
            "SHP_ORG_HULL", 
            "SHP_WEAPON_1_3",
            "GRO_SUBTER_HAB", 
            "LRN_ALGO_ELEGANCE", 
            "SHP_WEAPON_1_4",
            "PRO_ROBOTIC_PROD", 
            "LRN_ARTIF_MINDS", 
            "CON_ENV_ENCAPSUL", 
            "DEF_DEFENSE_NET_1", 
            "PRO_FUSION_GEN", 
            "PRO_INDUSTRY_CENTER_I", 
            "DEF_GARRISON_1", 
            "LRN_FORCE_FIELD",
            "CON_ORBITAL_CON", 
            "PRO_ORBITAL_GEN",
            "SHP_ZORTRIUM_PLATE",
            "GRO_SYMBIOTIC_BIO", 
            "SHP_WEAPON_2_1",
            "SHP_WEAPON_2_2",
            "SHP_DEFLECTOR_SHIELD", 
            "SHP_WEAPON_2_3",
            "DEF_DEFENSE_NET_2",
            "DEF_DEFENSE_NET_REGEN_1",
            "SPY_DETECT_2",
            "PRO_INDUSTRY_CENTER_II", 
            "PRO_SOL_ORB_GEN",
            "GRO_GENETIC_ENG", 
            "GRO_XENO_GENETICS", 
            "SHP_WEAPON_2_4",
            "DEF_GARRISON_2", 
            "CON_METRO_INFRA", 
            "LRN_QUANT_NET",
            "SHP_WEAPON_3_1",
            "GRO_LIFECYCLE_MAN", 
            "SHP_WEAPON_3_2",
            "SHP_WEAPON_3_3",
            "SHP_MULTICELL_CAST", 
            "SHP_WEAPON_3_4",
            "SHP_ENDOCRINE_SYSTEMS", 
            "DEF_PLAN_BARRIER_SHLD_1",
            "CON_FRC_ENRG_STRC", 
            "SPY_DETECT_3",
            "DEF_PLAN_BARRIER_SHLD_2",
            "LRN_GRAVITONICS",
            "CON_CONTGRAV_ARCH", 
            "LRN_PHYS_BRAIN", 
            "LRN_TRANSLING_THT", 
            "PRO_SENTIENT_AUTOMATION", 
            "PRO_EXOBOTS",
            "LRN_XENOARCH",
            "SHP_DIAMOND_PLATE",
            "DEF_DEFENSE_NET_3",
            "DEF_SYST_DEF_MINE_1",
            "DEF_PLAN_BARRIER_SHLD_3",
            "CON_ORBITAL_HAB",
            "DEF_GARRISON_3", 
            "SHP_CONT_SYMB",
            "SHP_MONOCELL_EXP",
            "SHP_BIOADAPTIVE_SPEC",
            "PRO_SINGULAR_GEN",
            "SHP_PLASMA_SHIELD", 
            "PRO_MICROGRAV_MAN",
            "GRO_XENO_HYBRIDS", 
            "LRN_TIME_MECH",
            "LRN_STELLAR_TOMOGRAPHY",
            "SPY_DETECT_4",
            "LRN_ENCLAVE_VOID",
            "LRN_PSY_DOM",
            "CON_NDIM_STRC", 
            "LRN_ART_BLACK_HOLE",
            "LRN_DISTRIB_THOUGHT", 
            "PRO_NEUTRONIUM_EXTRACTION",
            "SHP_XENTRONIUM_PLATE",
            "SHP_ASTEROID_HULLS",
            "GRO_TERRAFORM",
            "GRO_GENETIC_MED",
            "SHP_INTSTEL_LOG",
            "SHP_REINFORCED_HULL",
            "SHP_CONT_BIOADAPT",
            "SPY_STEALTH_1", 
            "GRO_CYBORG", 
            "SHP_BASIC_DAM_CONT",
            "CON_CONC_CAMP",
            "PRO_INDUSTRY_CENTER_III", 
            "SHP_SENT_HULL",
            "SPY_STEALTH_2", 
            "DEF_SYST_DEF_MINE_2",
            "SHP_ADV_DAM_CONT",
            "SHP_WEAPON_4_1",
            "SHP_WEAPON_4_2",
            "SHP_WEAPON_4_3",
            "DEF_SYST_DEF_MINE_3",
            "SPY_STEALTH_3", 
            "SPY_DETECT_5",
            "SHP_WEAPON_4_4",
            "SHP_BLACKSHIELD", 
            "DEF_GARRISON_4",
            "DEF_DEFENSE_NET_REGEN_2",
            "SPY_STEALTH_4", 
            "SHP_ENRG_BOUND_MAN", 
            "DEF_PLAN_BARRIER_SHLD_4",
            "DEF_PLAN_BARRIER_SHLD_5",
            "GRO_GAIA_TRANS",
            "GRO_ENERGY_META",
            "CON_ART_PLANET",
            "GRO_ENERGY_META", 
            "SHP_SOLAR_CONT",
            ]
            #"SHP_ENDOSYMB_HULL",
    elif index == 1:
        primaryMetaTechs = [
            "SHP_WEAPON_1_2",
            "GRO_PLANET_ECOL",
            "SHP_DOMESTIC_MONSTER",
            "SHP_ORG_HULL", 
            "SHP_WEAPON_1_3",
            "GRO_SUBTER_HAB", 
            "LRN_ALGO_ELEGANCE", 
            "SHP_WEAPON_1_4",
            "PRO_ROBOTIC_PROD", 
            "LRN_ARTIF_MINDS", 
            "CON_ENV_ENCAPSUL", 
            "DEF_DEFENSE_NET_1", 
            "PRO_FUSION_GEN", 
            "PRO_INDUSTRY_CENTER_I", 
            "DEF_GARRISON_1", 
            "CON_ORBITAL_CON", 
            "PRO_ORBITAL_GEN",
            "PRO_MICROGRAV_MAN",
            "SHP_ASTEROID_HULLS",
            "LRN_FORCE_FIELD",
            "GRO_SYMBIOTIC_BIO", 
            "SHP_WEAPON_2_1",
            "SHP_WEAPON_2_2",
            "SHP_DEFLECTOR_SHIELD", 
            "DEF_DEFENSE_NET_2",
            "DEF_DEFENSE_NET_REGEN_1",
            "SHP_ZORTRIUM_PLATE",
            "SPY_DETECT_2",
            "PRO_INDUSTRY_CENTER_II", 
            "PRO_SOL_ORB_GEN",
            "SHP_WEAPON_2_3",
            "GRO_GENETIC_ENG", 
            "GRO_XENO_GENETICS", 
            "SHP_WEAPON_2_4",
            "DEF_GARRISON_2", 
            "CON_METRO_INFRA", 
            "LRN_QUANT_NET",
            "SHP_WEAPON_3_1",
            "GRO_LIFECYCLE_MAN", 
            "SHP_WEAPON_3_2",
            "SHP_WEAPON_3_3",
            "SHP_MULTICELL_CAST", 
            "SHP_WEAPON_3_4",
            "SHP_ENDOCRINE_SYSTEMS", 
            "SHP_DIAMOND_PLATE",
            "DEF_PLAN_BARRIER_SHLD_1",
            "CON_FRC_ENRG_STRC", 
            "SPY_DETECT_3",
            "DEF_PLAN_BARRIER_SHLD_2",
            "LRN_GRAVITONICS",
            "CON_CONTGRAV_ARCH", 
            "LRN_PHYS_BRAIN", 
            "LRN_TRANSLING_THT", 
            "PRO_SENTIENT_AUTOMATION", 
            "PRO_EXOBOTS",
            "LRN_XENOARCH",
            "DEF_DEFENSE_NET_3",
            "DEF_SYST_DEF_MINE_1",
            "DEF_PLAN_BARRIER_SHLD_3",
            "CON_ORBITAL_HAB",
            "DEF_GARRISON_3", 
            "SHP_CONT_SYMB",
            "SHP_MONOCELL_EXP",
            "SHP_BIOADAPTIVE_SPEC",
            "PRO_SINGULAR_GEN",
            "SHP_PLASMA_SHIELD", 
            "GRO_XENO_HYBRIDS", 
            "LRN_TIME_MECH",
            "LRN_STELLAR_TOMOGRAPHY",
            "SPY_DETECT_4",
            "LRN_ENCLAVE_VOID",
            "LRN_PSY_DOM",
            "CON_NDIM_STRC", 
            "LRN_ART_BLACK_HOLE",
            "LRN_DISTRIB_THOUGHT", 
            "PRO_NEUTRONIUM_EXTRACTION",
            "SHP_XENTRONIUM_PLATE",
            "GRO_TERRAFORM",
            "GRO_GENETIC_MED",
            "SHP_INTSTEL_LOG",
            "SHP_REINFORCED_HULL",
            "SHP_CONT_BIOADAPT",
            "SPY_STEALTH_1", 
            "GRO_CYBORG", 
            "SHP_BASIC_DAM_CONT",
            "CON_CONC_CAMP",
            "PRO_INDUSTRY_CENTER_III", 
            "SHP_SENT_HULL",
            "SPY_STEALTH_2", 
            "DEF_SYST_DEF_MINE_2",
            "SHP_ADV_DAM_CONT",
            "SHP_WEAPON_4_1",
            "SHP_WEAPON_4_2",
            "SHP_WEAPON_4_3",
            "DEF_SYST_DEF_MINE_3",
            "SPY_STEALTH_3", 
            "SPY_DETECT_5",
            "SHP_WEAPON_4_4",
            "SHP_BLACKSHIELD", 
            "DEF_GARRISON_4",
            "DEF_DEFENSE_NET_REGEN_2",
            "SPY_STEALTH_4", 
            "SHP_ENRG_BOUND_MAN", 
            "DEF_PLAN_BARRIER_SHLD_4",
            "DEF_PLAN_BARRIER_SHLD_5",
            "GRO_GAIA_TRANS",
            "GRO_ENERGY_META",
            "CON_ART_PLANET",
            "GRO_ENERGY_META", 
            "SHP_SOLAR_CONT",
            ]
            #"CON_FRC_ENRG_STRC",
            #"CON_FRC_ENRG_CAMO",
            #"SHP_TITAN_HULL"
        
    return primaryMetaTechs
    
MasterTechList=[  # just for reference
        "CON_ARCH_MONOFILS",
        "CON_ARCH_PSYCH",
        "CON_ART_HEAVENLY",
        "CON_ART_PLANET",
        "CON_CONC_CAMP",
        "CON_CONTGRAV_ARCH",
        "CON_ENV_ENCAPSUL",
        "CON_FRC_ENRG_CAMO",
        "CON_FRC_ENRG_STRC",
        "CON_GAL_INFRA",
        "CON_INFRA_ECOL",
        "CON_METRO_INFRA",
        "CON_NDIM_STRC",
        "CON_ORBITAL_CON",
        "CON_ORBITAL_HAB",
        "CON_ORGANIC_STRC",
        "CON_PLANET_DRIVE",
        "CON_STARGATE",
        "CON_SUBTER_CONST",
        "CON_TRANS_ARCH",
        "DEF_DEFENSE_NET_1",
        "DEF_DEFENSE_NET_2",
        "DEF_DEFENSE_NET_3",
        "DEF_DEFENSE_NET_REGEN_1",
        "DEF_DEFENSE_NET_REGEN_2",
        "DEF_GARRISON_1",
        "DEF_GARRISON_2",
        "DEF_GARRISON_3",
        "DEF_GARRISON_4",
        "DEF_PLANET_CLOAK",
        "DEF_PLAN_BARRIER_SHLD_1",
        "DEF_PLAN_BARRIER_SHLD_2",
        "DEF_PLAN_BARRIER_SHLD_3",
        "DEF_PLAN_BARRIER_SHLD_4",
        "DEF_PLAN_BARRIER_SHLD_5",
        "DEF_ROOT_DEFENSE",
        "DEF_SYST_DEF_MINE_1",
        "DEF_SYST_DEF_MINE_2",
        "DEF_SYST_DEF_MINE_3",
        "GRO_ADV_ECOMAN",
        "GRO_BIOTERROR",
        "GRO_CYBORG",
        "GRO_ECO_METAMORPH",
        "GRO_ENERGY_META",
        "GRO_GAIA_TRANS",
        "GRO_GENETIC_ENG",
        "GRO_GENETIC_MED",
        "GRO_GENOME_BANK",
        "GRO_LIFECYCLE_MAN",
        "GRO_MEDICAL_PATH",
        "GRO_NANOTECH_MED",
        "GRO_NANO_CYBERNET",
        "GRO_PLANET_ECOL",
        "GRO_SUBTER_HAB",
        "GRO_SYMBIOTIC_BIO",
        "GRO_TERRAFORM",
        "GRO_TRANSORG_SENT",
        "GRO_XENO_GENETICS",
        "GRO_XENO_HYBRIDS",
        "LRN_ALGO_ELEGANCE",
        "LRN_ARTIF_MINDS",
        "LRN_ART_BLACK_HOLE",
        "LRN_DISTRIB_THOUGHT",
        "LRN_ENCLAVE_VOID",
        "LRN_EVERYTHING",
        "LRN_FORCE_FIELD",
        "LRN_GATEWAY_VOID",
        "LRN_GRAVITONICS",
        "LRN_MIND_VOID",
        "LRN_NDIM_SUBSPACE",
        "LRN_OBSERVATORY_I",
        "LRN_PHYS_BRAIN",
        "LRN_PSIONICS",
        "LRN_PSY_DOM",
        "LRN_QUANT_NET",
        "LRN_SPATIAL_DISTORT_GEN",
        "LRN_STELLAR_TOMOGRAPHY",
        "LRN_TIME_MECH",
        "LRN_TRANSCEND",
        "LRN_TRANSLING_THT",
        "LRN_UNIF_CONC",
        "LRN_XENOARCH",
        "PRO_CORE_MINE",
        "PRO_EXOBOTS",
        "PRO_FUSION_GEN",
        "PRO_INDUSTRY_CENTER_I",
        "PRO_INDUSTRY_CENTER_II",
        "PRO_INDUSTRY_CENTER_III",
        "PRO_MICROGRAV_MAN",
        "PRO_NANOTECH_PROD",
        "PRO_NDIM_ASSMB",
        "PRO_NEUTRONIUM_EXTRACTION",
        "PRO_ORBITAL_GEN",
        "PRO_ROBOTIC_PROD",
        "PRO_SENTIENT_AUTOMATION",
        "PRO_SINGULAR_GEN",
        "PRO_SOL_ORB_GEN",
        "PRO_ZERO_GEN",
        "SHP_ADV_DAM_CONT",
        "SHP_ANTIMATTER_TANK",
        "SHP_ASTEROID_HULLS",
        "SHP_ASTEROID_REFORM",
        "SHP_BASIC_DAM_CONT",
        "SHP_BIOADAPTIVE_SPEC",
        "SHP_BIOTERM",
        "SHP_CAMO_AST_HULL",
        "SHP_CONTGRAV_MAINT",
        "SHP_CONT_BIOADAPT",
        "SHP_CONT_SYMB",
        "SHP_DEATH_SPORE",
        "SHP_DEUTERIUM_TANK",
        "SHP_DIAMOND_PLATE",
        "SHP_DOMESTIC_MONSTER",
        "SHP_ENDOCRINE_SYSTEMS",
        "SHP_ENDOSYMB_HULL",
        "SHP_ENRG_BOUND_MAN",
        "SHP_FLEET_REPAIR",
        "SHP_FRC_ENRG_COMP",
        "SHP_GAL_EXPLO",
        "SHP_IMPROVED_ENGINE_COUPLINGS",
        "SHP_INTSTEL_LOG",
        "SHP_LEAD_PLATE",
        "SHP_MASSPROP_SPEC",
        "SHP_MIDCOMB_LOG",
        "SHP_MIL_ROBO_CONT",
        "SHP_MINIAST_SWARM",
        "SHP_MONOCELL_EXP",
        "SHP_MONOMOLEC_LATTICE",
        "SHP_MULTICELL_CAST",
        "SHP_MULTISPEC_SHIELD",
        "SHP_NANOROBO_MAINT",
        "SHP_NOVA_BOMB",
        "SHP_N_DIMENSIONAL_ENGINE_MATRIX",
        "SHP_ORG_HULL",
        "SHP_QUANT_ENRG_MAG",
        "SHP_REINFORCED_HULL",
        "SHP_ROOT_AGGRESSION",
        "SHP_SCAT_AST_HULL",
        "SHP_SENT_HULL",
        "SHP_SINGULARITY_ENGINE_CORE",
        "SHP_SOLAR_CONT",
        "SHP_SPACE_FLUX_DRIVE",
        "SHP_TRANSSPACE_DRIVE",
        "SHP_WEAPON_10",
        "SHP_WEAPON_11",
        "SHP_WEAPON_12",
        "SHP_WEAPON_13",
        "SHP_WEAPON_14",
        "SHP_WEAPON_15",
        "SHP_WEAPON_16",
        "SHP_WEAPON_17",
        "SHP_WEAPON_2",
        "SHP_WEAPON_3",
        "SHP_WEAPON_4",
        "SHP_WEAPON_5",
        "SHP_WEAPON_6",
        "SHP_WEAPON_7",
        "SHP_WEAPON_8",
        "SHP_WEAPON_9",
        "SHP_ZORTRIUM_PLATE",
        "SPY_DETECT_1",
        "SPY_DETECT_2",
        "SPY_DETECT_3",
        "SPY_DETECT_4",
        "SPY_DETECT_5",
        "SPY_DIST_MOD",
        "SPY_LIGHTHOUSE",
        "SPY_PLANET_STEALTH_MOD",
        "SPY_ROOT_DECEPTION",
        "SPY_STEALTH_1",
        "SPY_STEALTH_2",
        "SPY_STEALTH_3",
        "SPY_STEALTH_4",
        ]
