GameRule(
    name="RULE_SHIP_HULL_COST_FACTOR",
    description="RULE_SHIP_HULL_COST_FACTOR_DESC",
    category="BALANCE",
    type=float,
    default=1.0,
    min=0.1,
    max=10.0,
)

GameRule(
    name="RULE_SHIP_PART_COST_FACTOR",
    description="RULE_SHIP_PART_COST_FACTOR_DESC",
    category="BALANCE",
    type=float,
    default=1.0,
    min=0.1,
    max=10.0,
)

GameRule(
    name="RULE_TECH_COST_FACTOR",
    description="RULE_TECH_COST_FACTOR_DESC",
    category="BALANCE",
    type=float,
    default=1.0,
    min=0.1,
    max=10.0,
)

GameRule(
    name="RULE_BUILDING_COST_FACTOR",
    description="RULE_BUILDING_COST_FACTOR_DESC",
    category="BALANCE",
    type=float,
    default=1.0,
    min=0.1,
    max=10.0,
)

GameRule(
    name="RULE_SINGULARITY_COST_FACTOR",
    description="RULE_SINGULARITY_COST_FACTOR_DESC",
    category="BALANCE",
    type=float,
    default=1.0,
    min=0.1,
    max=10.0,
)

GameRule(
    name="RULE_ENABLE_EXPERIMENTORS",
    description="RULE_ENABLE_EXPERIMENTORS_DESC",
    category="CONTENT",
    type=bool,
    default=True,
)

GameRule(
    name="RULE_EXPERIMENTORS_SPAWN_BASE_TURN",
    description="RULE_EXPERIMENTORS_SPAWN_BASE_TURN_DESC",
    category="CONTENT",
    type=int,
    default=250,
    min=0,
    max=500,
)

GameRule(
    name="RULE_ENABLE_SUPER_TESTER",
    description="RULE_ENABLE_SUPER_TESTER_DESC",
    category="CONTENT",
    type=bool,
    default=True,
)

GameRule(
    name="RULE_TEST_STRING",
    description="RULE_TEST_STRING_DESC",
    category="TEST",
    type=str,
    default="PLAYER",
    allowed=["MODERATOR", "OBSERVER", "PLAYER", "AI_PLAYER"],
)

GameRule(
    name="RULE_HABITABLE_SIZE_TINY",
    description="RULE_HABITABLE_SIZE_DESC",
    category="PLANET_SIZE",
    type=int,
    default=1,
    min=0,
    max=999,
)

GameRule(
    name="RULE_HABITABLE_SIZE_SMALL",
    description="RULE_HABITABLE_SIZE_DESC",
    category="PLANET_SIZE",
    type=int,
    default=2,
    min=0,
    max=999,
)

GameRule(
    name="RULE_HABITABLE_SIZE_MEDIUM",
    description="RULE_HABITABLE_SIZE_DESC",
    category="PLANET_SIZE",
    type=int,
    default=3,
    min=0,
    max=999,
)

GameRule(
    name="RULE_HABITABLE_SIZE_LARGE",
    description="RULE_HABITABLE_SIZE_DESC",
    category="PLANET_SIZE",
    type=int,
    default=4,
    min=0,
    max=999,
)

GameRule(
    name="RULE_HABITABLE_SIZE_HUGE",
    description="RULE_HABITABLE_SIZE_DESC",
    category="PLANET_SIZE",
    type=int,
    default=5,
    min=0,
    max=999,
)

GameRule(
    name="RULE_HABITABLE_SIZE_ASTEROIDS",
    description="RULE_HABITABLE_SIZE_DESC",
    category="PLANET_SIZE",
    type=int,
    default=3,
    min=0,
    max=999,
)

GameRule(
    name="RULE_HABITABLE_SIZE_GASGIANT",
    description="RULE_HABITABLE_SIZE_DESC",
    category="PLANET_SIZE",
    type=int,
    default=6,
    min=0,
    max=999,
)

GameRule(
    name="RULE_BASELINE_PLANET_STABILITY",
    description="RULE_BASELINE_PLANET_STABILITY_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=0,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_PROTECTION_FOCUS_STABILITY",
    description="RULE_PROTECTION_FOCUS_STABILITY_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=15,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_IMPERIAL_PALACE_INFLUENCE",
    description="RULE_IMPERIAL_PALACE_INFLUENCE_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=3,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_GOOD_ENVIRONMENT_STABILITY",
    description="RULE_GOOD_ENVIRONMENT_STABILITY_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=2,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_ADEQUATE_ENVIRONMENT_STABILITY",
    description="RULE_ADEQUATE_ENVIRONMENT_STABILITY_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=1,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_POOR_ENVIRONMENT_STABILITY",
    description="RULE_POOR_ENVIRONMENT_STABILITY_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=0,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_HOSTILE_ENVIRONMENT_STABILITY",
    description="RULE_HOSTILE_ENVIRONMENT_STABILITY_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=-1,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_TINY_SIZE_STABILITY",
    description="RULE_TINY_SIZE_STABILITY_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=2,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_SMALL_SIZE_STABILITY",
    description="RULE_SMALL_SIZE_STABILITY_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=1,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_MEDIUM_SIZE_STABILITY",
    description="RULE_MEDIUM_SIZE_STABILITY_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=0,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_LARGE_SIZE_STABILITY",
    description="RULE_LARGE_SIZE_STABILITY_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=-1,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_HUGE_SIZE_STABILITY",
    description="RULE_HUGE_SIZE_STABILITY_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=-2,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_GAS_GIANT_SIZE_STABILITY",
    description="RULE_GAS_GIANT_SIZE_STABILITY_DESC",
    category="BALANCE_STABILITY",
    type=int,
    default=0,
    min=-20,
    max=20,
)

GameRule(
    name="RULE_SHIP_PART_BASED_UPKEEP",
    description="RULE_SHIP_PART_BASED_UPKEEP_DESC",
    category="BALANCE",
    type=bool,
    default=False,
)

GameRule(
    name="RULE_ENABLE_ALLIED_REPAIR",
    description="RULE_ENABLE_ALLIED_REPAIR_DESC",
    category="MULTIPLAYER",
    type=bool,
    default=False,
)

GameRule(
    name="RULE_FIRST_COMBAT_ROUND_IN_CLOSE_TARGETING_RANGE",
    description="RULE_FIRST_COMBAT_ROUND_IN_CLOSE_TARGETING_RANGE_DESC",
    category="BALANCE",
    type=int,
    default=3,
    min=1,
    max=20,
)

GameRule(
    name="RULE_ALLOW_REPEATED_SPECIES",
    description="RULE_ALLOW_REPEATED_SPECIES_DESC",
    category="CONTENT",
    type=bool,
    default=False,
)

GameRule(
    name="RULE_ENSURE_HABITABLE_PLANET_HW_VICINITY",
    description="RULE_ENSURE_HABITABLE_PLANET_HW_VICINITY_DESC",
    category="CONTENT",
    type=bool,
    default=False,
)

GameRule(
    name="RULE_MINIMUM_MONSTER_DISTANCE_CAPITAL",
    description="RULE_MINIMUM_MONSTER_DISTANCE_CAPITAL_DESC",
    category="BALANCE",
    type=int,
    default=3,
    min=1,
    max=10,
)
