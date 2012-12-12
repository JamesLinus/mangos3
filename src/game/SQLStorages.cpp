/*
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "SQLStorages.h"

//const char CreatureInfosrcfmt[] = "iiiiiiiiiisssiiiiibiiiiiifffiffiifiiiiiiiiiiffiiiiiiiiiiiiiiisiiffliiiiiiiliiiiiis";
//const char CreatureInfodstfmt[] = "iiiiiiiiiisssiiiiibiiiiiifffiffiifiiiiiiiiiiffiiiiiiiiiiiiiiisiiffliiiiiiiliiiiiii";
const char CreatureInfosrcfmt[] = "iiiiiiiiiisssiiiiiiiiiiiifffiffiifiiiiiiiiiiiffiiiiiiiiiiiiiiiiiiisiiffiiiiiiiiiiiiiiis";
const char CreatureInfodstfmt[] = "iiiiiiiiiisssiiiiiiiiiiiifffiffiifiiiiiiiiiiiffiiiiiiiiiiiiiiiiiiisiiffiiiiiiiiiiiiiiis";
const char CreatureDataAddonInfofmt[] = "iiibbiis";
const char CreatureModelfmt[] = "iffbii";
const char CreatureInfoAddonInfofmt[] = "iiibbiis";
const char GameObjectInfoAddonInfofmt[] = "iffff";
const char EquipmentInfofmt[] = "iiii";
const char GameObjectInfosrcfmt[] = "iiissssiifiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiis";
const char GameObjectInfodstfmt[] = "iiissssiifiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
const char ItemPrototypesrcfmt[]= "iiiisiiiiffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiifiiifiiiiiifiiiiiifiiiiiifiiiiiifiiiisiiiiiiiiiiiiiiiiiiiiiiiifiiisiifiiiii";
const char ItemPrototypedstfmt[]= "iiiisiiiiffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiifiiifiiiiiifiiiiiifiiiiiifiiiiiifiiiisiiiiiiiiiiiiiiiiiiiiiiiifiiiiiifiiiii";
const char PageTextfmt[] = "isi";
const char InstanceTemplatesrcfmt[] = "iiiis";
const char InstanceTemplatedstfmt[] = "iiiii";
const char WorldTemplatesrcfmt[] = "is";
const char WorldTemplatedstfmt[] = "ii";
const char ConditionsSrcFmt[] = "iiii";
const char ConditionsDstFmt[] = "iiii";
const char SpellTemplatesrcfmt[] = "iiiiiiiiiiix";
//                                  0         10        20        30        40        50        60        70        80        90        100       110       120       130       140       150       160     170       180  185
const char SpellTemplatedstfmt[] = "ixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxiixxxxixxxxxxFxxxxxxxxxxxxxxxxxxxxxxixxxxxFFFxxxxxxixxxxxixxixxxxxFFFxxxxxxixxixxixxFFFxxxxxxxxxxxxxppppppppppppppppppppppppppppppppxxxxxxxxxxxFFFxxxxxFFFx";
//                                Id                              proc  DurationIndex                 Effect0        tarA0    effectAura0          triggerSpell0      SpellName[16]   Rank[16]
//                                     0         10        20        30        40        50        60        70        80        90        100       110       120
const char SpellDbcTemplatesrcfmt[] = "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiifiiiiiiiiiifffiiiiiiiiiiiiiiiiiiiiifffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiifffiiix";
const char SpellDbcTemplatedstfmt[] = "iiiiiiiiiiiiiiixixiiiiiiiiiiiiixiiiiiiiiiiiiifixxxxxxxxxxxxxxxxxxiiiiiiiiifffiiiiiiiiiiiiiiiiiiiiifffxxxiixiiiiiiiiiFFFiiiiiiiiixxixppppppppppppppppppppppppppppppppiiiiiiiiiiifffxxiiiFFFx";

const char VehicleAccessorySrcFmt[] = "iiiiffffx";
const char VehicleAccessoryDstFmt[] = "iiiiffff";

const char CreatureTemplateSpellsFmt[] = "iiiiiiiii";

SQLStorage sCreatureStorage(CreatureInfosrcfmt, CreatureInfodstfmt, "entry", "creature_template");
SQLStorage sCreatureDataAddonStorage(CreatureDataAddonInfofmt, "guid", "creature_addon");
SQLStorage sCreatureModelStorage(CreatureModelfmt, "modelid", "creature_model_info");
SQLStorage sCreatureInfoAddonStorage(CreatureInfoAddonInfofmt, "entry", "creature_template_addon");
SQLStorage sEquipmentStorage(EquipmentInfofmt, "entry", "creature_equip_template");
SQLStorage sItemStorage(ItemPrototypesrcfmt, ItemPrototypedstfmt, "entry", "item_template");
SQLStorage sPageTextStore(PageTextfmt, "entry", "page_text");
SQLStorage sInstanceTemplate(InstanceTemplatesrcfmt, InstanceTemplatedstfmt, "map", "instance_template");
SQLStorage sWorldTemplate(WorldTemplatesrcfmt, WorldTemplatedstfmt, "map", "world_template");
SQLStorage sConditionStorage(ConditionsSrcFmt, ConditionsDstFmt, "condition_entry", "conditions");

SQLHashStorage sGameObjectDataAddonStorage(GameObjectInfoAddonInfofmt, "guid", "gameobject_addon");
SQLHashStorage sSpellDbcTemplate(SpellDbcTemplatesrcfmt, SpellDbcTemplatedstfmt, "Id", "spell_dbc");
SQLHashStorage sGOStorage(GameObjectInfosrcfmt, GameObjectInfodstfmt, "entry", "gameobject_template");
SQLHashStorage sSpellTemplate(SpellTemplatesrcfmt, SpellTemplatedstfmt, "id", "spell_template");
//SQLHashStorage sCreatureTemplateSpellsStorage(CreatureTemplateSpellsFmt, "entry", "creature_template_spells");

SQLMultiStorage sVehicleAccessoryStorage(VehicleAccessorySrcFmt, VehicleAccessoryDstFmt, "vehicle_entry", "vehicle_accessory");