# Macro, global, alias and constant index

Every macro, global variable, alias, using declaration and Python module
constant found in the tracked sources. Generated state aliases are omitted
because the state field index covers the same names. The value column is the
definition. An empty description means the declaration has no leading comment.

| Symbol | Kind | File:Line | Value | Description |
| --- | --- | --- | --- | --- |
| `CC_BASE` | macro | `engine/gen/cc_map.h:6` | `#define CC_BASE 16u` |  |
| `CC_PARAMS` | macro | `engine/gen/cc_map.h:7` | `#define CC_PARAMS 55u` |  |
| `CC_NONE` | macro | `engine/gen/cc_map.h:8` | `#define CC_NONE 0xFFFFu` |  |
| `cc_to_param` | global | `engine/gen/cc_map.h:9` | `static const uint16_t cc_to_param[128] = { 65535u, 65535u, 65535u, 65535u, 65535u, 65535u, 65535u, 65535u, 65535u, 65...` |  |
| `param_to_cc` | global | `engine/gen/cc_map.h:28` | `static const uint8_t param_to_cc[2053] = { 21u, 255u, 23u, 255u, 255u, 255u, 255u, 255u, 255u, 255u, 255u, 255u, 255u...` |  |
| `param_min` | global | `engine/gen/cc_map.h:203` | `static const int32_t param_min[2053] = { 32768, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0...` |  |
| `param_max` | global | `engine/gen/cc_map.h:463` | `static const int32_t param_max[2053] = { 1048576, 50, 14, 1, 1, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 9...` |  |
| `DSP_TAP_FRAMES` | macro | `engine/gen/dsp.h:7` | `#define DSP_TAP_FRAMES 256u` |  |
| `NVOICE` | macro | `engine/gen/dsp.h:8` | `#define NVOICE 24` |  |
| `N_PART` | macro | `engine/gen/dsp.h:9` | `#define N_PART 2` |  |
| `CONTROL_BLOCK` | macro | `engine/gen/dsp.h:10` | `#define CONTROL_BLOCK 32` |  |
| `type_p_alpha_N_TAP_CH` | macro | `engine/gen/dsp.h:11` | `#define type_p_alpha_N_TAP_CH 10u` |  |
| `N_TAP_CH` | macro | `engine/gen/dsp.h:12` | `#define N_TAP_CH 10u` |  |
| `type_p_alpha_INIT_N` | macro | `engine/gen/dsp.h:2155` | `#define type_p_alpha_INIT_N (sizeof type_p_alpha_init / sizeof type_p_alpha_init[0])` |  |
| `p1_sin_q15` | global | `engine/gen/math_tables.h:8` | `static const int16_t p1_sin_q15[256] = { 0, 201, 402, 603, 804, 1005, 1206, 1407, 1608, 1809, 2009, 2210, 2411, 2611,...` |  |
| `p1_exp2_q16` | global | `engine/gen/math_tables.h:27` | `static const int32_t p1_exp2_q16[256] = { 65536, 65359, 65182, 65006, 64830, 64655, 64480, 64306, 64132, 63958, 63785...` |  |
| `p1_log2_q16` | global | `engine/gen/math_tables.h:62` | `static const int32_t p1_log2_q16[256] = { 0, 369, 736, 1102, 1466, 1829, 2190, 2551, 2909, 3267, 3623, 3978, 4331, 46...` |  |
| `p1_semi_q16` | global | `engine/gen/math_tables.h:97` | `static const int32_t p1_semi_q16[12] = { 65536, 69433, 73562, 77936, 82570, 87480, 92682, 98193, 104032, 110218, 1167...` |  |
| `p1_cent_q16` | global | `engine/gen/math_tables.h:102` | `static const int32_t p1_cent_q16[100] = { 65536, 65574, 65612, 65650, 65688, 65726, 65764, 65802, 65840, 65878, 65916...` |  |
| `p1_detune_pitch` | global | `engine/gen/math_tables.h:118` | `static const int32_t p1_detune_pitch[101] = { -933, -914, -896, -878, -859, -841, -822, -804, -785, -767, -748, -730,...` |  |
| `p1_rate_coeff` | global | `engine/gen/math_tables.h:134` | `static const int32_t p1_rate_coeff[100] = { 6, 6, 6, 7, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 13, 13, 14, 15, 16, 17,...` |  |
| `p1_rate_coeff_r3` | global | `engine/gen/math_tables.h:150` | `static const int32_t p1_rate_coeff_r3[100] = { 2, 3, 4, 4, 5, 5, 6, 6, 6, 7, 8, 9, 10, 11, 12, 14, 15, 17, 18, 20, 22...` |  |
| `p1_level_gain` | global | `engine/gen/math_tables.h:166` | `static const int32_t p1_level_gain[100] = { 0, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18, 20, 21, 23, 25, 28, 30, 33...` |  |
| `p1_filter_g_q16` | global | `engine/gen/math_tables.h:182` | `static const int32_t p1_filter_g_q16[257] = { 30, 31, 32, 33, 34, 35, 36, 38, 39, 40, 42, 43, 44, 46, 47, 49, 50, 52,...` |  |
| `p1_naive_wave` | global | `engine/gen/naive_wave.h:14` | `static const int16_t p1_naive_wave[14336] = { -2080, -6148, -10135, -13987, -17656, -21096, -24269, -27140, -29683, -...` |  |
| `p1_naive_base` | global | `engine/gen/naive_wave.h:913` | `static const uint8_t p1_naive_base[4] = { 0, 6, 7, 8, }` |  |
| `table_routing` | global | `engine/gen/tables.c:4` | `const int16_t table_routing[200] = { 0, 0, 0, 0, 32767, 0, 0, 0, 0, 32767, 0, 0, 0, 0, 32767, 0, 0, 0, 0, 32767, 0, 0...` |  |
| `table_routing_len` | macro | `engine/gen/tables.h:5` | `#define table_routing_len 200` |  |
| `table_routing` | global | `engine/gen/tables.h:6` | `extern const int16_t table_routing[200]` |  |
| `P1_DSP_API` | macro | `engine/gen/type_p_alpha_dsp.c:6` | `#define P1_DSP_API` |  |
| `NVOICE` | macro | `engine/gen/type_p_alpha_dsp.c:8` | `#define NVOICE 24` |  |
| `N_PART` | macro | `engine/gen/type_p_alpha_dsp.c:9` | `#define N_PART 2` |  |
| `CONTROL_BLOCK` | macro | `engine/gen/type_p_alpha_dsp.c:10` | `#define CONTROL_BLOCK 32` |  |
| `DSP_TAP_FRAMES` | macro | `engine/gen/type_p_alpha_dsp.c:11` | `#define DSP_TAP_FRAMES 256u` |  |
| `N_TAP_CH` | macro | `engine/gen/type_p_alpha_dsp.c:12` | `#define N_TAP_CH 10u` |  |
| `note_inc_c9` | global | `engine/gen/type_p_alpha_dsp.c:1677` | `static const uint32_t note_inc_c9[12] = { 749115498u, 793660223u, 840853716u, 890853480u, 943826385u, 999949221u, 105...` |  |
| `pan_l_table` | global | `engine/gen/type_p_alpha_dsp.c:1684` | `static const int32_t pan_l_table[201] = { 92682, 92679, 92670, 92656, 92636, 92610, 92579, 92542, 92499, 92450, 92396...` |  |
| `pan_r_table` | global | `engine/gen/type_p_alpha_dsp.c:1713` | `static const int32_t pan_r_table[201] = { 0, 728, 1456, 2184, 2911, 3639, 4366, 5093, 5820, 6546, 7272, 7997, 8722, 9...` |  |
| `PARAM_COUNT` | macro | `engine/gen/type_p_alpha_dsp.c:1742` | `#define PARAM_COUNT 2053` |  |
| `type_p_alpha_dsp_params` | global | `engine/gen/type_p_alpha_dsp.c:1746` | `const dsp_param type_p_alpha_dsp_params[2053] = { { 0u, "op1_ratio.p0", 32768, 1048576, 65536, 0u, 0u, "" }, { 1u, "o...` |  |
| `type_p_alpha_slots` | global | `engine/gen/type_p_alpha_dsp.c:3802` | `static const type_p_alpha_slot_t type_p_alpha_slots[2053] = { { (uint32_t)offsetof(type_p_alpha_state, op1_ph_inc_rat...` |  |
| `d1_prev` | macro | `engine/gen/type_p_alpha_dsp.c:6274` | `#define d1_prev (S->d1_prev)` |  |
| `d2_prev` | macro | `engine/gen/type_p_alpha_dsp.c:6275` | `#define d2_prev (S->d2_prev)` |  |
| `d3_prev` | macro | `engine/gen/type_p_alpha_dsp.c:6276` | `#define d3_prev (S->d3_prev)` |  |
| `d4_prev` | macro | `engine/gen/type_p_alpha_dsp.c:6277` | `#define d4_prev (S->d4_prev)` |  |
| `penv_stage` | macro | `engine/gen/type_p_alpha_dsp.c:6278` | `#define penv_stage (S->penv_stage)` |  |
| `penv_level` | macro | `engine/gen/type_p_alpha_dsp.c:6279` | `#define penv_level (S->penv_level)` |  |
| `penv_r1_eff` | macro | `engine/gen/type_p_alpha_dsp.c:6280` | `#define penv_r1_eff (S->penv_r1_eff)` |  |
| `penv_r2_eff` | macro | `engine/gen/type_p_alpha_dsp.c:6281` | `#define penv_r2_eff (S->penv_r2_eff)` |  |
| `penv_r3_eff` | macro | `engine/gen/type_p_alpha_dsp.c:6282` | `#define penv_r3_eff (S->penv_r3_eff)` |  |
| `penv_r4_eff` | macro | `engine/gen/type_p_alpha_dsp.c:6283` | `#define penv_r4_eff (S->penv_r4_eff)` |  |
| `penv_rate_macro` | macro | `engine/gen/type_p_alpha_dsp.c:6284` | `#define penv_rate_macro (S->penv_rate_macro)` |  |
| `penv_r1_base` | macro | `engine/gen/type_p_alpha_dsp.c:6285` | `#define penv_r1_base (S->penv_r1_base)` |  |
| `penv_r2_base` | macro | `engine/gen/type_p_alpha_dsp.c:6286` | `#define penv_r2_base (S->penv_r2_base)` |  |
| `penv_r3_base` | macro | `engine/gen/type_p_alpha_dsp.c:6287` | `#define penv_r3_base (S->penv_r3_base)` |  |
| `penv_relr_base` | macro | `engine/gen/type_p_alpha_dsp.c:6288` | `#define penv_relr_base (S->penv_relr_base)` |  |
| `penv_ks_k` | macro | `engine/gen/type_p_alpha_dsp.c:6289` | `#define penv_ks_k (S->penv_ks_k)` |  |
| `penv_vs_k` | macro | `engine/gen/type_p_alpha_dsp.c:6290` | `#define penv_vs_k (S->penv_vs_k)` |  |
| `plfo_phase` | macro | `engine/gen/type_p_alpha_dsp.c:6291` | `#define plfo_phase (S->plfo_phase)` |  |
| `plfo_base` | macro | `engine/gen/type_p_alpha_dsp.c:6292` | `#define plfo_base (S->plfo_base)` |  |
| `plfo_w` | macro | `engine/gen/type_p_alpha_dsp.c:6293` | `#define plfo_w (S->plfo_w)` |  |
| `plfo_rate_level` | macro | `engine/gen/type_p_alpha_dsp.c:6294` | `#define plfo_rate_level (S->plfo_rate_level)` |  |
| `plfo_depth_level` | macro | `engine/gen/type_p_alpha_dsp.c:6295` | `#define plfo_depth_level (S->plfo_depth_level)` |  |
| `plfo_rate_step` | macro | `engine/gen/type_p_alpha_dsp.c:6296` | `#define plfo_rate_step (S->plfo_rate_step)` |  |
| `plfo_depth_step` | macro | `engine/gen/type_p_alpha_dsp.c:6297` | `#define plfo_depth_step (S->plfo_depth_step)` |  |
| `plfo_rate_ratio` | macro | `engine/gen/type_p_alpha_dsp.c:6298` | `#define plfo_rate_ratio (S->plfo_rate_ratio)` |  |
| `plfo_depth_macro` | macro | `engine/gen/type_p_alpha_dsp.c:6299` | `#define plfo_depth_macro (S->plfo_depth_macro)` |  |
| `f1_ks_k` | macro | `engine/gen/type_p_alpha_dsp.c:6300` | `#define f1_ks_k (S->f1_ks_k)` |  |
| `fenv_ks_k` | macro | `engine/gen/type_p_alpha_dsp.c:6301` | `#define fenv_ks_k (S->fenv_ks_k)` |  |
| `f1_vs_k` | macro | `engine/gen/type_p_alpha_dsp.c:6302` | `#define f1_vs_k (S->f1_vs_k)` |  |
| `fenv_vs_k` | macro | `engine/gen/type_p_alpha_dsp.c:6303` | `#define fenv_vs_k (S->fenv_vs_k)` |  |
| `lp_z1` | macro | `engine/gen/type_p_alpha_dsp.c:6304` | `#define lp_z1 (S->lp_z1)` |  |
| `lp_z2` | macro | `engine/gen/type_p_alpha_dsp.c:6305` | `#define lp_z2 (S->lp_z2)` |  |
| `lp_z3` | macro | `engine/gen/type_p_alpha_dsp.c:6306` | `#define lp_z3 (S->lp_z3)` |  |
| `lp_z4` | macro | `engine/gen/type_p_alpha_dsp.c:6307` | `#define lp_z4 (S->lp_z4)` |  |
| `lp_fc` | macro | `engine/gen/type_p_alpha_dsp.c:6308` | `#define lp_fc (S->lp_fc)` |  |
| `lp_res_k` | macro | `engine/gen/type_p_alpha_dsp.c:6309` | `#define lp_res_k (S->lp_res_k)` |  |
| `lp_env` | macro | `engine/gen/type_p_alpha_dsp.c:6310` | `#define lp_env (S->lp_env)` |  |
| `lp_gate` | macro | `engine/gen/type_p_alpha_dsp.c:6311` | `#define lp_gate (S->lp_gate)` |  |
| `fenv_stage` | macro | `engine/gen/type_p_alpha_dsp.c:6312` | `#define fenv_stage (S->fenv_stage)` |  |
| `fenv_level` | macro | `engine/gen/type_p_alpha_dsp.c:6313` | `#define fenv_level (S->fenv_level)` |  |
| `fenv_r1_eff` | macro | `engine/gen/type_p_alpha_dsp.c:6314` | `#define fenv_r1_eff (S->fenv_r1_eff)` |  |
| `fenv_r2_eff` | macro | `engine/gen/type_p_alpha_dsp.c:6315` | `#define fenv_r2_eff (S->fenv_r2_eff)` |  |
| `fenv_r3_eff` | macro | `engine/gen/type_p_alpha_dsp.c:6316` | `#define fenv_r3_eff (S->fenv_r3_eff)` |  |
| `fenv_r4_eff` | macro | `engine/gen/type_p_alpha_dsp.c:6317` | `#define fenv_r4_eff (S->fenv_r4_eff)` |  |
| `fenv_rate_macro` | macro | `engine/gen/type_p_alpha_dsp.c:6318` | `#define fenv_rate_macro (S->fenv_rate_macro)` |  |
| `fenv_r1_base` | macro | `engine/gen/type_p_alpha_dsp.c:6319` | `#define fenv_r1_base (S->fenv_r1_base)` |  |
| `fenv_r2_base` | macro | `engine/gen/type_p_alpha_dsp.c:6320` | `#define fenv_r2_base (S->fenv_r2_base)` |  |
| `fenv_r3_base` | macro | `engine/gen/type_p_alpha_dsp.c:6321` | `#define fenv_r3_base (S->fenv_r3_base)` |  |
| `fenv_relr_base` | macro | `engine/gen/type_p_alpha_dsp.c:6322` | `#define fenv_relr_base (S->fenv_relr_base)` |  |
| `dc_x1` | macro | `engine/gen/type_p_alpha_dsp.c:6323` | `#define dc_x1 (S->dc_x1)` |  |
| `dc_y1` | macro | `engine/gen/type_p_alpha_dsp.c:6324` | `#define dc_y1 (S->dc_y1)` |  |
| `voice_age` | macro | `engine/gen/type_p_alpha_dsp.c:6325` | `#define voice_age (S->voice_age)` |  |
| `voice_velocity` | macro | `engine/gen/type_p_alpha_dsp.c:6326` | `#define voice_velocity (S->voice_velocity)` |  |
| `voice_delay` | macro | `engine/gen/type_p_alpha_dsp.c:6327` | `#define voice_delay (S->voice_delay)` |  |
| `voice_inc` | macro | `engine/gen/type_p_alpha_dsp.c:6328` | `#define voice_inc (S->voice_inc)` |  |
| `voice_target_inc` | macro | `engine/gen/type_p_alpha_dsp.c:6329` | `#define voice_target_inc (S->voice_target_inc)` |  |
| `voice_detune` | macro | `engine/gen/type_p_alpha_dsp.c:6330` | `#define voice_detune (S->voice_detune)` |  |
| `mono_order` | macro | `engine/gen/type_p_alpha_dsp.c:6331` | `#define mono_order (S->mono_order)` |  |
| `mono_serial` | macro | `engine/gen/type_p_alpha_dsp.c:6332` | `#define mono_serial (S->mono_serial)` |  |
| `part_bend_q16` | macro | `engine/gen/type_p_alpha_dsp.c:6333` | `#define part_bend_q16 (S->part_bend_q16)` |  |
| `pan_l_q16` | macro | `engine/gen/type_p_alpha_dsp.c:6334` | `#define pan_l_q16 (S->pan_l_q16)` |  |
| `pan_r_q16` | macro | `engine/gen/type_p_alpha_dsp.c:6335` | `#define pan_r_q16 (S->pan_r_q16)` |  |
| `pan_env_level` | macro | `engine/gen/type_p_alpha_dsp.c:6336` | `#define pan_env_level (S->pan_env_level)` |  |
| `ages` | macro | `engine/gen/type_p_alpha_dsp.c:6337` | `#define ages (S->ages)` |  |
| `vm_mode` | macro | `engine/gen/type_p_alpha_dsp.c:6338` | `#define vm_mode (S->vm_mode)` |  |
| `vm_unison_count` | macro | `engine/gen/type_p_alpha_dsp.c:6339` | `#define vm_unison_count (S->vm_unison_count)` |  |
| `vm_unison_detune` | macro | `engine/gen/type_p_alpha_dsp.c:6340` | `#define vm_unison_detune (S->vm_unison_detune)` |  |
| `vm_detune` | macro | `engine/gen/type_p_alpha_dsp.c:6341` | `#define vm_detune (S->vm_detune)` |  |
| `vm_transpose` | macro | `engine/gen/type_p_alpha_dsp.c:6342` | `#define vm_transpose (S->vm_transpose)` |  |
| `vm_legato` | macro | `engine/gen/type_p_alpha_dsp.c:6343` | `#define vm_legato (S->vm_legato)` |  |
| `vm_portamento` | macro | `engine/gen/type_p_alpha_dsp.c:6344` | `#define vm_portamento (S->vm_portamento)` |  |
| `vm_note_priority` | macro | `engine/gen/type_p_alpha_dsp.c:6345` | `#define vm_note_priority (S->vm_note_priority)` |  |
| `vm_bend_range` | macro | `engine/gen/type_p_alpha_dsp.c:6346` | `#define vm_bend_range (S->vm_bend_range)` |  |
| `vm_pan` | macro | `engine/gen/type_p_alpha_dsp.c:6347` | `#define vm_pan (S->vm_pan)` |  |
| `vm_pan_env_on` | macro | `engine/gen/type_p_alpha_dsp.c:6348` | `#define vm_pan_env_on (S->vm_pan_env_on)` |  |
| `vm_pan_env_r1` | macro | `engine/gen/type_p_alpha_dsp.c:6349` | `#define vm_pan_env_r1 (S->vm_pan_env_r1)` |  |
| `vm_pan_env_r2` | macro | `engine/gen/type_p_alpha_dsp.c:6350` | `#define vm_pan_env_r2 (S->vm_pan_env_r2)` |  |
| `vm_pan_env_r3` | macro | `engine/gen/type_p_alpha_dsp.c:6351` | `#define vm_pan_env_r3 (S->vm_pan_env_r3)` |  |
| `vm_pan_env_l1` | macro | `engine/gen/type_p_alpha_dsp.c:6352` | `#define vm_pan_env_l1 (S->vm_pan_env_l1)` |  |
| `vm_pan_env_l2` | macro | `engine/gen/type_p_alpha_dsp.c:6353` | `#define vm_pan_env_l2 (S->vm_pan_env_l2)` |  |
| `vm_pan_env_l3` | macro | `engine/gen/type_p_alpha_dsp.c:6354` | `#define vm_pan_env_l3 (S->vm_pan_env_l3)` |  |
| `vm_pan_env_relr` | macro | `engine/gen/type_p_alpha_dsp.c:6355` | `#define vm_pan_env_relr (S->vm_pan_env_relr)` |  |
| `vm_pan_env_rell` | macro | `engine/gen/type_p_alpha_dsp.c:6356` | `#define vm_pan_env_rell (S->vm_pan_env_rell)` |  |
| `vm_pan_env_mode` | macro | `engine/gen/type_p_alpha_dsp.c:6357` | `#define vm_pan_env_mode (S->vm_pan_env_mode)` |  |
| `part_channel` | macro | `engine/gen/type_p_alpha_dsp.c:6358` | `#define part_channel (S->part_channel)` |  |
| `part_low` | macro | `engine/gen/type_p_alpha_dsp.c:6359` | `#define part_low (S->part_low)` |  |
| `part_high` | macro | `engine/gen/type_p_alpha_dsp.c:6360` | `#define part_high (S->part_high)` |  |
| `part_level` | macro | `engine/gen/type_p_alpha_dsp.c:6361` | `#define part_level (S->part_level)` |  |
| `part_delay` | macro | `engine/gen/type_p_alpha_dsp.c:6362` | `#define part_delay (S->part_delay)` |  |
| `algo_algorithm` | macro | `engine/gen/type_p_alpha_dsp.c:7283` | `#define algo_algorithm (S->algo_algorithm)` |  |
| `route_ring1_on` | macro | `engine/gen/type_p_alpha_dsp.c:7284` | `#define route_ring1_on (S->route_ring1_on)` |  |
| `route_ring1_level` | macro | `engine/gen/type_p_alpha_dsp.c:7285` | `#define route_ring1_level (S->route_ring1_level)` |  |
| `route_ring2_on` | macro | `engine/gen/type_p_alpha_dsp.c:7286` | `#define route_ring2_on (S->route_ring2_on)` |  |
| `route_ring2_level` | macro | `engine/gen/type_p_alpha_dsp.c:7287` | `#define route_ring2_level (S->route_ring2_level)` |  |
| `route_ring3_on` | macro | `engine/gen/type_p_alpha_dsp.c:7288` | `#define route_ring3_on (S->route_ring3_on)` |  |
| `route_ring3_level` | macro | `engine/gen/type_p_alpha_dsp.c:7289` | `#define route_ring3_level (S->route_ring3_level)` |  |
| `route_car1_out` | macro | `engine/gen/type_p_alpha_dsp.c:7290` | `#define route_car1_out (S->route_car1_out)` |  |
| `route_car2_out` | macro | `engine/gen/type_p_alpha_dsp.c:7291` | `#define route_car2_out (S->route_car2_out)` |  |
| `route_car3_out` | macro | `engine/gen/type_p_alpha_dsp.c:7292` | `#define route_car3_out (S->route_car3_out)` |  |
| `route_car4_out` | macro | `engine/gen/type_p_alpha_dsp.c:7293` | `#define route_car4_out (S->route_car4_out)` |  |
| `penv_on` | macro | `engine/gen/type_p_alpha_dsp.c:7294` | `#define penv_on (S->penv_on)` |  |
| `penv_r1` | macro | `engine/gen/type_p_alpha_dsp.c:7295` | `#define penv_r1 (S->penv_r1)` |  |
| `penv_r2` | macro | `engine/gen/type_p_alpha_dsp.c:7296` | `#define penv_r2 (S->penv_r2)` |  |
| `penv_r3` | macro | `engine/gen/type_p_alpha_dsp.c:7297` | `#define penv_r3 (S->penv_r3)` |  |
| `penv_l1` | macro | `engine/gen/type_p_alpha_dsp.c:7298` | `#define penv_l1 (S->penv_l1)` |  |
| `penv_l2` | macro | `engine/gen/type_p_alpha_dsp.c:7299` | `#define penv_l2 (S->penv_l2)` |  |
| `penv_l3` | macro | `engine/gen/type_p_alpha_dsp.c:7300` | `#define penv_l3 (S->penv_l3)` |  |
| `penv_relr` | macro | `engine/gen/type_p_alpha_dsp.c:7301` | `#define penv_relr (S->penv_relr)` |  |
| `penv_rell` | macro | `engine/gen/type_p_alpha_dsp.c:7302` | `#define penv_rell (S->penv_rell)` |  |
| `penv_mode` | macro | `engine/gen/type_p_alpha_dsp.c:7303` | `#define penv_mode (S->penv_mode)` |  |
| `penv_ks_lo` | macro | `engine/gen/type_p_alpha_dsp.c:7304` | `#define penv_ks_lo (S->penv_ks_lo)` |  |
| `penv_ks_hi` | macro | `engine/gen/type_p_alpha_dsp.c:7305` | `#define penv_ks_hi (S->penv_ks_hi)` |  |
| `penv_vs_amount` | macro | `engine/gen/type_p_alpha_dsp.c:7306` | `#define penv_vs_amount (S->penv_vs_amount)` |  |
| `plfo_rate` | macro | `engine/gen/type_p_alpha_dsp.c:7307` | `#define plfo_rate (S->plfo_rate)` |  |
| `plfo_depth` | macro | `engine/gen/type_p_alpha_dsp.c:7308` | `#define plfo_depth (S->plfo_depth)` |  |
| `plfo_wave` | macro | `engine/gen/type_p_alpha_dsp.c:7309` | `#define plfo_wave (S->plfo_wave)` |  |
| `plfo_mode` | macro | `engine/gen/type_p_alpha_dsp.c:7310` | `#define plfo_mode (S->plfo_mode)` |  |
| `plfo_rate_fade` | macro | `engine/gen/type_p_alpha_dsp.c:7311` | `#define plfo_rate_fade (S->plfo_rate_fade)` |  |
| `plfo_depth_fade` | macro | `engine/gen/type_p_alpha_dsp.c:7312` | `#define plfo_depth_fade (S->plfo_depth_fade)` |  |
| `f1_ks_lo` | macro | `engine/gen/type_p_alpha_dsp.c:7313` | `#define f1_ks_lo (S->f1_ks_lo)` |  |
| `f1_ks_hi` | macro | `engine/gen/type_p_alpha_dsp.c:7314` | `#define f1_ks_hi (S->f1_ks_hi)` |  |
| `fenv_ks_lo` | macro | `engine/gen/type_p_alpha_dsp.c:7315` | `#define fenv_ks_lo (S->fenv_ks_lo)` |  |
| `fenv_ks_hi` | macro | `engine/gen/type_p_alpha_dsp.c:7316` | `#define fenv_ks_hi (S->fenv_ks_hi)` |  |
| `f1_vs_amount` | macro | `engine/gen/type_p_alpha_dsp.c:7317` | `#define f1_vs_amount (S->f1_vs_amount)` |  |
| `fenv_vs_amount` | macro | `engine/gen/type_p_alpha_dsp.c:7318` | `#define fenv_vs_amount (S->fenv_vs_amount)` |  |
| `cutoff_pos_cutoff` | macro | `engine/gen/type_p_alpha_dsp.c:7319` | `#define cutoff_pos_cutoff (S->cutoff_pos_cutoff)` |  |
| `lp_res` | macro | `engine/gen/type_p_alpha_dsp.c:7320` | `#define lp_res (S->lp_res)` |  |
| `lp_comp` | macro | `engine/gen/type_p_alpha_dsp.c:7321` | `#define lp_comp (S->lp_comp)` |  |
| `lp_mode` | macro | `engine/gen/type_p_alpha_dsp.c:7322` | `#define lp_mode (S->lp_mode)` |  |
| `lp_on` | macro | `engine/gen/type_p_alpha_dsp.c:7323` | `#define lp_on (S->lp_on)` |  |
| `fenv_on` | macro | `engine/gen/type_p_alpha_dsp.c:7324` | `#define fenv_on (S->fenv_on)` |  |
| `fenv_r1` | macro | `engine/gen/type_p_alpha_dsp.c:7325` | `#define fenv_r1 (S->fenv_r1)` |  |
| `fenv_r2` | macro | `engine/gen/type_p_alpha_dsp.c:7326` | `#define fenv_r2 (S->fenv_r2)` |  |
| `fenv_r3` | macro | `engine/gen/type_p_alpha_dsp.c:7327` | `#define fenv_r3 (S->fenv_r3)` |  |
| `fenv_l1` | macro | `engine/gen/type_p_alpha_dsp.c:7328` | `#define fenv_l1 (S->fenv_l1)` |  |
| `fenv_l2` | macro | `engine/gen/type_p_alpha_dsp.c:7329` | `#define fenv_l2 (S->fenv_l2)` |  |
| `fenv_l3` | macro | `engine/gen/type_p_alpha_dsp.c:7330` | `#define fenv_l3 (S->fenv_l3)` |  |
| `fenv_relr` | macro | `engine/gen/type_p_alpha_dsp.c:7331` | `#define fenv_relr (S->fenv_relr)` |  |
| `fenv_rell` | macro | `engine/gen/type_p_alpha_dsp.c:7332` | `#define fenv_rell (S->fenv_rell)` |  |
| `fenv_mode` | macro | `engine/gen/type_p_alpha_dsp.c:7333` | `#define fenv_mode (S->fenv_mode)` |  |
| `key_bp_bp` | macro | `engine/gen/type_p_alpha_dsp.c:7334` | `#define key_bp_bp (S->key_bp_bp)` |  |
| `key_on_on` | macro | `engine/gen/type_p_alpha_dsp.c:7335` | `#define key_on_on (S->key_on_on)` |  |
| `vel_on_on` | macro | `engine/gen/type_p_alpha_dsp.c:7336` | `#define vel_on_on (S->vel_on_on)` |  |
| `dc_r` | macro | `engine/gen/type_p_alpha_dsp.c:7337` | `#define dc_r (S->dc_r)` |  |
| `macro_g1` | macro | `engine/gen/type_p_alpha_dsp.c:7338` | `#define macro_g1 (S->macro_g1)` |  |
| `macro_g2` | macro | `engine/gen/type_p_alpha_dsp.c:7339` | `#define macro_g2 (S->macro_g2)` |  |
| `macro_g3` | macro | `engine/gen/type_p_alpha_dsp.c:7340` | `#define macro_g3 (S->macro_g3)` |  |
| `macro_g4` | macro | `engine/gen/type_p_alpha_dsp.c:7341` | `#define macro_g4 (S->macro_g4)` |  |
| `macro_g5` | macro | `engine/gen/type_p_alpha_dsp.c:7342` | `#define macro_g5 (S->macro_g5)` |  |
| `macro_g6` | macro | `engine/gen/type_p_alpha_dsp.c:7343` | `#define macro_g6 (S->macro_g6)` |  |
| `macro_s1` | macro | `engine/gen/type_p_alpha_dsp.c:7344` | `#define macro_s1 (S->macro_s1)` |  |
| `macro_s2` | macro | `engine/gen/type_p_alpha_dsp.c:7345` | `#define macro_s2 (S->macro_s2)` |  |
| `macro_s3` | macro | `engine/gen/type_p_alpha_dsp.c:7346` | `#define macro_s3 (S->macro_s3)` |  |
| `macro_s4` | macro | `engine/gen/type_p_alpha_dsp.c:7347` | `#define macro_s4 (S->macro_s4)` |  |
| `macro_s5` | macro | `engine/gen/type_p_alpha_dsp.c:7348` | `#define macro_s5 (S->macro_s5)` |  |
| `macro_s6` | macro | `engine/gen/type_p_alpha_dsp.c:7349` | `#define macro_s6 (S->macro_s6)` |  |
| `macro_sr1` | macro | `engine/gen/type_p_alpha_dsp.c:7350` | `#define macro_sr1 (S->macro_sr1)` |  |
| `macro_sr2` | macro | `engine/gen/type_p_alpha_dsp.c:7351` | `#define macro_sr2 (S->macro_sr2)` |  |
| `macro_sr3` | macro | `engine/gen/type_p_alpha_dsp.c:7352` | `#define macro_sr3 (S->macro_sr3)` |  |
| `macro_sr4` | macro | `engine/gen/type_p_alpha_dsp.c:7353` | `#define macro_sr4 (S->macro_sr4)` |  |
| `macro_sr5` | macro | `engine/gen/type_p_alpha_dsp.c:7354` | `#define macro_sr5 (S->macro_sr5)` |  |
| `macro_sr6` | macro | `engine/gen/type_p_alpha_dsp.c:7355` | `#define macro_sr6 (S->macro_sr6)` |  |
| `macro_t1a` | macro | `engine/gen/type_p_alpha_dsp.c:7356` | `#define macro_t1a (S->macro_t1a)` |  |
| `macro_t1b` | macro | `engine/gen/type_p_alpha_dsp.c:7357` | `#define macro_t1b (S->macro_t1b)` |  |
| `macro_t1c` | macro | `engine/gen/type_p_alpha_dsp.c:7358` | `#define macro_t1c (S->macro_t1c)` |  |
| `macro_t2a` | macro | `engine/gen/type_p_alpha_dsp.c:7359` | `#define macro_t2a (S->macro_t2a)` |  |
| `macro_t2b` | macro | `engine/gen/type_p_alpha_dsp.c:7360` | `#define macro_t2b (S->macro_t2b)` |  |
| `macro_t2c` | macro | `engine/gen/type_p_alpha_dsp.c:7361` | `#define macro_t2c (S->macro_t2c)` |  |
| `macro_t3a` | macro | `engine/gen/type_p_alpha_dsp.c:7362` | `#define macro_t3a (S->macro_t3a)` |  |
| `macro_t3b` | macro | `engine/gen/type_p_alpha_dsp.c:7363` | `#define macro_t3b (S->macro_t3b)` |  |
| `macro_t3c` | macro | `engine/gen/type_p_alpha_dsp.c:7364` | `#define macro_t3c (S->macro_t3c)` |  |
| `macro_t4a` | macro | `engine/gen/type_p_alpha_dsp.c:7365` | `#define macro_t4a (S->macro_t4a)` |  |
| `macro_t4b` | macro | `engine/gen/type_p_alpha_dsp.c:7366` | `#define macro_t4b (S->macro_t4b)` |  |
| `macro_t4c` | macro | `engine/gen/type_p_alpha_dsp.c:7367` | `#define macro_t4c (S->macro_t4c)` |  |
| `macro_t5a` | macro | `engine/gen/type_p_alpha_dsp.c:7368` | `#define macro_t5a (S->macro_t5a)` |  |
| `macro_t5b` | macro | `engine/gen/type_p_alpha_dsp.c:7369` | `#define macro_t5b (S->macro_t5b)` |  |
| `macro_t5c` | macro | `engine/gen/type_p_alpha_dsp.c:7370` | `#define macro_t5c (S->macro_t5c)` |  |
| `macro_t6a` | macro | `engine/gen/type_p_alpha_dsp.c:7371` | `#define macro_t6a (S->macro_t6a)` |  |
| `macro_t6b` | macro | `engine/gen/type_p_alpha_dsp.c:7372` | `#define macro_t6b (S->macro_t6b)` |  |
| `macro_t6c` | macro | `engine/gen/type_p_alpha_dsp.c:7373` | `#define macro_t6c (S->macro_t6c)` |  |
| `macro_r1a` | macro | `engine/gen/type_p_alpha_dsp.c:7374` | `#define macro_r1a (S->macro_r1a)` |  |
| `macro_r1b` | macro | `engine/gen/type_p_alpha_dsp.c:7375` | `#define macro_r1b (S->macro_r1b)` |  |
| `macro_r1c` | macro | `engine/gen/type_p_alpha_dsp.c:7376` | `#define macro_r1c (S->macro_r1c)` |  |
| `macro_r2a` | macro | `engine/gen/type_p_alpha_dsp.c:7377` | `#define macro_r2a (S->macro_r2a)` |  |
| `macro_r2b` | macro | `engine/gen/type_p_alpha_dsp.c:7378` | `#define macro_r2b (S->macro_r2b)` |  |
| `macro_r2c` | macro | `engine/gen/type_p_alpha_dsp.c:7379` | `#define macro_r2c (S->macro_r2c)` |  |
| `macro_r3a` | macro | `engine/gen/type_p_alpha_dsp.c:7380` | `#define macro_r3a (S->macro_r3a)` |  |
| `macro_r3b` | macro | `engine/gen/type_p_alpha_dsp.c:7381` | `#define macro_r3b (S->macro_r3b)` |  |
| `macro_r3c` | macro | `engine/gen/type_p_alpha_dsp.c:7382` | `#define macro_r3c (S->macro_r3c)` |  |
| `macro_r4a` | macro | `engine/gen/type_p_alpha_dsp.c:7383` | `#define macro_r4a (S->macro_r4a)` |  |
| `macro_r4b` | macro | `engine/gen/type_p_alpha_dsp.c:7384` | `#define macro_r4b (S->macro_r4b)` |  |
| `macro_r4c` | macro | `engine/gen/type_p_alpha_dsp.c:7385` | `#define macro_r4c (S->macro_r4c)` |  |
| `macro_r5a` | macro | `engine/gen/type_p_alpha_dsp.c:7386` | `#define macro_r5a (S->macro_r5a)` |  |
| `macro_r5b` | macro | `engine/gen/type_p_alpha_dsp.c:7387` | `#define macro_r5b (S->macro_r5b)` |  |
| `macro_r5c` | macro | `engine/gen/type_p_alpha_dsp.c:7388` | `#define macro_r5c (S->macro_r5c)` |  |
| `macro_r6a` | macro | `engine/gen/type_p_alpha_dsp.c:7389` | `#define macro_r6a (S->macro_r6a)` |  |
| `macro_r6b` | macro | `engine/gen/type_p_alpha_dsp.c:7390` | `#define macro_r6b (S->macro_r6b)` |  |
| `macro_r6c` | macro | `engine/gen/type_p_alpha_dsp.c:7391` | `#define macro_r6c (S->macro_r6c)` |  |
| `ctrl_serial` | macro | `engine/gen/type_p_alpha_dsp.c:7392` | `#define ctrl_serial (S->ctrl_serial)` |  |
| `patch_gate` | macro | `engine/gen/type_p_alpha_dsp.c:7393` | `#define patch_gate (S->patch_gate)` |  |
| `patch_trig` | macro | `engine/gen/type_p_alpha_dsp.c:7394` | `#define patch_trig (S->patch_trig)` |  |
| `dcr_x1` | macro | `engine/gen/type_p_alpha_dsp.c:7395` | `#define dcr_x1 (S->dcr_x1)` |  |
| `dcr_y1` | macro | `engine/gen/type_p_alpha_dsp.c:7396` | `#define dcr_y1 (S->dcr_y1)` |  |
| `vel_on_on_x` | macro | `engine/gen/type_p_alpha_dsp.c:7397` | `#define vel_on_on_x (S->vel_on_on_x)` |  |
| `penv_vs_k_x` | macro | `engine/gen/type_p_alpha_dsp.c:7398` | `#define penv_vs_k_x (S->penv_vs_k_x)` |  |
| `fenv_vs_k_x` | macro | `engine/gen/type_p_alpha_dsp.c:7411` | `#define fenv_vs_k_x (S->fenv_vs_k_x)` |  |
| `f1_vs_k_x` | macro | `engine/gen/type_p_alpha_dsp.c:7412` | `#define f1_vs_k_x (S->f1_vs_k_x)` |  |
| `key_on_on_x` | macro | `engine/gen/type_p_alpha_dsp.c:7413` | `#define key_on_on_x (S->key_on_on_x)` |  |
| `key_bp_bp_x` | macro | `engine/gen/type_p_alpha_dsp.c:7414` | `#define key_bp_bp_x (S->key_bp_bp_x)` |  |
| `penv_ks_k_x` | macro | `engine/gen/type_p_alpha_dsp.c:7415` | `#define penv_ks_k_x (S->penv_ks_k_x)` |  |
| `penv_cv_x` | macro | `engine/gen/type_p_alpha_dsp.c:7416` | `#define penv_cv_x (S->penv_cv_x)` |  |
| `penv_active_x` | macro | `engine/gen/type_p_alpha_dsp.c:7417` | `#define penv_active_x (S->penv_active_x)` |  |
| `fenv_ks_k_x` | macro | `engine/gen/type_p_alpha_dsp.c:7430` | `#define fenv_ks_k_x (S->fenv_ks_k_x)` |  |
| `f1_ks_k_x` | macro | `engine/gen/type_p_alpha_dsp.c:7431` | `#define f1_ks_k_x (S->f1_ks_k_x)` |  |
| `cutoff_pos_f_x` | macro | `engine/gen/type_p_alpha_dsp.c:7432` | `#define cutoff_pos_f_x (S->cutoff_pos_f_x)` |  |
| `cutoff_pos_pos_x` | macro | `engine/gen/type_p_alpha_dsp.c:7433` | `#define cutoff_pos_pos_x (S->cutoff_pos_pos_x)` |  |
| `fenv_cv_x` | macro | `engine/gen/type_p_alpha_dsp.c:7434` | `#define fenv_cv_x (S->fenv_cv_x)` |  |
| `fenv_active_x` | macro | `engine/gen/type_p_alpha_dsp.c:7435` | `#define fenv_active_x (S->fenv_active_x)` |  |
| `plfo_cv_x` | macro | `engine/gen/type_p_alpha_dsp.c:7436` | `#define plfo_cv_x (S->plfo_cv_x)` |  |
| `psum_cv_x` | macro | `engine/gen/type_p_alpha_dsp.c:7437` | `#define psum_cv_x (S->psum_cv_x)` |  |
| `algo_alg_x` | macro | `engine/gen/type_p_alpha_dsp.c:7438` | `#define algo_alg_x (S->algo_alg_x)` |  |
| `type_p_alpha_render_bus0` | macro | `engine/gen/type_p_alpha_dsp.c:7511` | `#define type_p_alpha_render_bus0 (S->type_p_alpha_render_bus0)` |  |
| `type_p_alpha_render_bus1` | macro | `engine/gen/type_p_alpha_dsp.c:7512` | `#define type_p_alpha_render_bus1 (S->type_p_alpha_render_bus1)` |  |
| `type_p_alpha_tap0` | macro | `engine/gen/type_p_alpha_dsp.c:7513` | `#define type_p_alpha_tap0 (S->type_p_alpha_tap0)` |  |
| `type_p_alpha_tap1` | macro | `engine/gen/type_p_alpha_dsp.c:7514` | `#define type_p_alpha_tap1 (S->type_p_alpha_tap1)` |  |
| `voice_rel` | macro | `engine/gen/type_p_alpha_dsp.c:7531` | `#define voice_rel (S->voice_rel)` |  |
| `voice_gate` | macro | `engine/gen/type_p_alpha_dsp.c:7532` | `#define voice_gate (S->voice_gate)` |  |
| `voice_note` | macro | `engine/gen/type_p_alpha_dsp.c:7533` | `#define voice_note (S->voice_note)` |  |
| `voice_trig` | macro | `engine/gen/type_p_alpha_dsp.c:7534` | `#define voice_trig (S->voice_trig)` |  |
| `voice_part` | macro | `engine/gen/type_p_alpha_dsp.c:7535` | `#define voice_part (S->voice_part)` |  |
| `mono_held` | macro | `engine/gen/type_p_alpha_dsp.c:7536` | `#define mono_held (S->mono_held)` |  |
| `part_mono_voice` | macro | `engine/gen/type_p_alpha_dsp.c:7537` | `#define part_mono_voice (S->part_mono_voice)` |  |
| `part_mono_count` | macro | `engine/gen/type_p_alpha_dsp.c:7538` | `#define part_mono_count (S->part_mono_count)` |  |
| `pan_env_stage` | macro | `engine/gen/type_p_alpha_dsp.c:7539` | `#define pan_env_stage (S->pan_env_stage)` |  |
| `pan_env_trig` | macro | `engine/gen/type_p_alpha_dsp.c:7540` | `#define pan_env_trig (S->pan_env_trig)` |  |
| `P1_STATE_PTR` | macro | `engine/gen/type_p_alpha_dsp.c:7543` | `#define P1_STATE_PTR` |  |
| `type_p_alpha_state` | global | `engine/gen/type_p_alpha_dsp.c:7545` | `static P1_STATE_PTR type_p_alpha_state` |  |
| `P1_PE_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:8047` | `#define P1_PE_WORDS 64u` | Per-part signed phase envelope. The pool is one per operator, indexed by the part; the first active voice advances it once per control block. A trigger on any voice of the part restarts the attack. The levels are absolute percent of the breakpoint field. |
| `P1_PE_STAGE` | macro | `engine/gen/type_p_alpha_dsp.c:8048` | `#define P1_PE_STAGE 0u` |  |
| `P1_PE_LEVEL` | macro | `engine/gen/type_p_alpha_dsp.c:8049` | `#define P1_PE_LEVEL 1u` |  |
| `P1_PE_LASTB` | macro | `engine/gen/type_p_alpha_dsp.c:8050` | `#define P1_PE_LASTB 2u` |  |
| `P1_PE_SEEDED` | macro | `engine/gen/type_p_alpha_dsp.c:8051` | `#define P1_PE_SEEDED 3u` |  |
| `P1_PE_HADGATE` | macro | `engine/gen/type_p_alpha_dsp.c:8052` | `#define P1_PE_HADGATE 4u` |  |
| `P1_PL_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:8177` | `#define P1_PL_WORDS 64u` | Per-part phase LFO. The pool is one per operator, indexed by the part. The first active voice on a part advances it once per control block and every voice reads the held value. The LFO free-runs, so a note does not reset its phase. The output is percent of the breakpoint field. |
| `P1_PL_PHASE` | macro | `engine/gen/type_p_alpha_dsp.c:8178` | `#define P1_PL_PHASE 0u` |  |
| `P1_PL_BASE` | macro | `engine/gen/type_p_alpha_dsp.c:8179` | `#define P1_PL_BASE 1u` |  |
| `P1_PL_RATE_LEVEL` | macro | `engine/gen/type_p_alpha_dsp.c:8180` | `#define P1_PL_RATE_LEVEL 2u` |  |
| `P1_PL_DEPTH_LEVEL` | macro | `engine/gen/type_p_alpha_dsp.c:8181` | `#define P1_PL_DEPTH_LEVEL 3u` |  |
| `P1_PL_RATE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:8182` | `#define P1_PL_RATE_STEP 4u` |  |
| `P1_PL_DEPTH_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:8183` | `#define P1_PL_DEPTH_STEP 5u` |  |
| `P1_PL_VALUE` | macro | `engine/gen/type_p_alpha_dsp.c:8184` | `#define P1_PL_VALUE 6u` |  |
| `P1_PL_LASTB` | macro | `engine/gen/type_p_alpha_dsp.c:8185` | `#define P1_PL_LASTB 7u` |  |
| `P1_PL_SEEDED` | macro | `engine/gen/type_p_alpha_dsp.c:8186` | `#define P1_PL_SEEDED 8u` |  |
| `P1_PL_RAND` | macro | `engine/gen/type_p_alpha_dsp.c:8187` | `#define P1_PL_RAND 9u` |  |
| `P1_PL_HOLD` | macro | `engine/gen/type_p_alpha_dsp.c:8188` | `#define P1_PL_HOLD 10u` |  |
| `P1_PE_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:8744` | `#define P1_PE_WORDS 64u` | Per-part signed phase envelope. The pool is one per operator, indexed by the part; the first active voice advances it once per control block. A trigger on any voice of the part restarts the attack. The levels are absolute percent of the breakpoint field. |
| `P1_PE_STAGE` | macro | `engine/gen/type_p_alpha_dsp.c:8745` | `#define P1_PE_STAGE 0u` |  |
| `P1_PE_LEVEL` | macro | `engine/gen/type_p_alpha_dsp.c:8746` | `#define P1_PE_LEVEL 1u` |  |
| `P1_PE_LASTB` | macro | `engine/gen/type_p_alpha_dsp.c:8747` | `#define P1_PE_LASTB 2u` |  |
| `P1_PE_SEEDED` | macro | `engine/gen/type_p_alpha_dsp.c:8748` | `#define P1_PE_SEEDED 3u` |  |
| `P1_PE_HADGATE` | macro | `engine/gen/type_p_alpha_dsp.c:8749` | `#define P1_PE_HADGATE 4u` |  |
| `P1_PL_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:8874` | `#define P1_PL_WORDS 64u` | Per-part phase LFO. The pool is one per operator, indexed by the part. The first active voice on a part advances it once per control block and every voice reads the held value. The LFO free-runs, so a note does not reset its phase. The output is percent of the breakpoint field. |
| `P1_PL_PHASE` | macro | `engine/gen/type_p_alpha_dsp.c:8875` | `#define P1_PL_PHASE 0u` |  |
| `P1_PL_BASE` | macro | `engine/gen/type_p_alpha_dsp.c:8876` | `#define P1_PL_BASE 1u` |  |
| `P1_PL_RATE_LEVEL` | macro | `engine/gen/type_p_alpha_dsp.c:8877` | `#define P1_PL_RATE_LEVEL 2u` |  |
| `P1_PL_DEPTH_LEVEL` | macro | `engine/gen/type_p_alpha_dsp.c:8878` | `#define P1_PL_DEPTH_LEVEL 3u` |  |
| `P1_PL_RATE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:8879` | `#define P1_PL_RATE_STEP 4u` |  |
| `P1_PL_DEPTH_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:8880` | `#define P1_PL_DEPTH_STEP 5u` |  |
| `P1_PL_VALUE` | macro | `engine/gen/type_p_alpha_dsp.c:8881` | `#define P1_PL_VALUE 6u` |  |
| `P1_PL_LASTB` | macro | `engine/gen/type_p_alpha_dsp.c:8882` | `#define P1_PL_LASTB 7u` |  |
| `P1_PL_SEEDED` | macro | `engine/gen/type_p_alpha_dsp.c:8883` | `#define P1_PL_SEEDED 8u` |  |
| `P1_PL_RAND` | macro | `engine/gen/type_p_alpha_dsp.c:8884` | `#define P1_PL_RAND 9u` |  |
| `P1_PL_HOLD` | macro | `engine/gen/type_p_alpha_dsp.c:8885` | `#define P1_PL_HOLD 10u` |  |
| `P1_PE_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:9441` | `#define P1_PE_WORDS 64u` | Per-part signed phase envelope. The pool is one per operator, indexed by the part; the first active voice advances it once per control block. A trigger on any voice of the part restarts the attack. The levels are absolute percent of the breakpoint field. |
| `P1_PE_STAGE` | macro | `engine/gen/type_p_alpha_dsp.c:9442` | `#define P1_PE_STAGE 0u` |  |
| `P1_PE_LEVEL` | macro | `engine/gen/type_p_alpha_dsp.c:9443` | `#define P1_PE_LEVEL 1u` |  |
| `P1_PE_LASTB` | macro | `engine/gen/type_p_alpha_dsp.c:9444` | `#define P1_PE_LASTB 2u` |  |
| `P1_PE_SEEDED` | macro | `engine/gen/type_p_alpha_dsp.c:9445` | `#define P1_PE_SEEDED 3u` |  |
| `P1_PE_HADGATE` | macro | `engine/gen/type_p_alpha_dsp.c:9446` | `#define P1_PE_HADGATE 4u` |  |
| `P1_PL_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:9571` | `#define P1_PL_WORDS 64u` | Per-part phase LFO. The pool is one per operator, indexed by the part. The first active voice on a part advances it once per control block and every voice reads the held value. The LFO free-runs, so a note does not reset its phase. The output is percent of the breakpoint field. |
| `P1_PL_PHASE` | macro | `engine/gen/type_p_alpha_dsp.c:9572` | `#define P1_PL_PHASE 0u` |  |
| `P1_PL_BASE` | macro | `engine/gen/type_p_alpha_dsp.c:9573` | `#define P1_PL_BASE 1u` |  |
| `P1_PL_RATE_LEVEL` | macro | `engine/gen/type_p_alpha_dsp.c:9574` | `#define P1_PL_RATE_LEVEL 2u` |  |
| `P1_PL_DEPTH_LEVEL` | macro | `engine/gen/type_p_alpha_dsp.c:9575` | `#define P1_PL_DEPTH_LEVEL 3u` |  |
| `P1_PL_RATE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:9576` | `#define P1_PL_RATE_STEP 4u` |  |
| `P1_PL_DEPTH_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:9577` | `#define P1_PL_DEPTH_STEP 5u` |  |
| `P1_PL_VALUE` | macro | `engine/gen/type_p_alpha_dsp.c:9578` | `#define P1_PL_VALUE 6u` |  |
| `P1_PL_LASTB` | macro | `engine/gen/type_p_alpha_dsp.c:9579` | `#define P1_PL_LASTB 7u` |  |
| `P1_PL_SEEDED` | macro | `engine/gen/type_p_alpha_dsp.c:9580` | `#define P1_PL_SEEDED 8u` |  |
| `P1_PL_RAND` | macro | `engine/gen/type_p_alpha_dsp.c:9581` | `#define P1_PL_RAND 9u` |  |
| `P1_PL_HOLD` | macro | `engine/gen/type_p_alpha_dsp.c:9582` | `#define P1_PL_HOLD 10u` |  |
| `P1_PE_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:10138` | `#define P1_PE_WORDS 64u` | Per-part signed phase envelope. The pool is one per operator, indexed by the part; the first active voice advances it once per control block. A trigger on any voice of the part restarts the attack. The levels are absolute percent of the breakpoint field. |
| `P1_PE_STAGE` | macro | `engine/gen/type_p_alpha_dsp.c:10139` | `#define P1_PE_STAGE 0u` |  |
| `P1_PE_LEVEL` | macro | `engine/gen/type_p_alpha_dsp.c:10140` | `#define P1_PE_LEVEL 1u` |  |
| `P1_PE_LASTB` | macro | `engine/gen/type_p_alpha_dsp.c:10141` | `#define P1_PE_LASTB 2u` |  |
| `P1_PE_SEEDED` | macro | `engine/gen/type_p_alpha_dsp.c:10142` | `#define P1_PE_SEEDED 3u` |  |
| `P1_PE_HADGATE` | macro | `engine/gen/type_p_alpha_dsp.c:10143` | `#define P1_PE_HADGATE 4u` |  |
| `P1_PL_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:10268` | `#define P1_PL_WORDS 64u` | Per-part phase LFO. The pool is one per operator, indexed by the part. The first active voice on a part advances it once per control block and every voice reads the held value. The LFO free-runs, so a note does not reset its phase. The output is percent of the breakpoint field. |
| `P1_PL_PHASE` | macro | `engine/gen/type_p_alpha_dsp.c:10269` | `#define P1_PL_PHASE 0u` |  |
| `P1_PL_BASE` | macro | `engine/gen/type_p_alpha_dsp.c:10270` | `#define P1_PL_BASE 1u` |  |
| `P1_PL_RATE_LEVEL` | macro | `engine/gen/type_p_alpha_dsp.c:10271` | `#define P1_PL_RATE_LEVEL 2u` |  |
| `P1_PL_DEPTH_LEVEL` | macro | `engine/gen/type_p_alpha_dsp.c:10272` | `#define P1_PL_DEPTH_LEVEL 3u` |  |
| `P1_PL_RATE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:10273` | `#define P1_PL_RATE_STEP 4u` |  |
| `P1_PL_DEPTH_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:10274` | `#define P1_PL_DEPTH_STEP 5u` |  |
| `P1_PL_VALUE` | macro | `engine/gen/type_p_alpha_dsp.c:10275` | `#define P1_PL_VALUE 6u` |  |
| `P1_PL_LASTB` | macro | `engine/gen/type_p_alpha_dsp.c:10276` | `#define P1_PL_LASTB 7u` |  |
| `P1_PL_SEEDED` | macro | `engine/gen/type_p_alpha_dsp.c:10277` | `#define P1_PL_SEEDED 8u` |  |
| `P1_PL_RAND` | macro | `engine/gen/type_p_alpha_dsp.c:10278` | `#define P1_PL_RAND 9u` |  |
| `P1_PL_HOLD` | macro | `engine/gen/type_p_alpha_dsp.c:10279` | `#define P1_PL_HOLD 10u` |  |
| `P1_PM_NX` | macro | `engine/gen/type_p_alpha_dsp.c:11435` | `#define P1_PM_NX 0u /* 33 x uint32: natural input boundaries 0..4096 */` | The pool starts at 2,560 words per part. The optional direct-read table is appended per part, so P1_PM_WORDS is defined after the table mode below. |
| `P1_PM_NSLOPE` | macro | `engine/gen/type_p_alpha_dsp.c:11436` | `#define P1_PM_NSLOPE 33u /* 32 x uint32: natural slopes, Q16 */` | #define P1_PM_NX 0u          /* 33 x uint32: natural input boundaries 0..4096 |
| `P1_PM_NFLIP` | macro | `engine/gen/type_p_alpha_dsp.c:11437` | `#define P1_PM_NFLIP 65u /* 32 x uint32: natural segment orientation */` | #define P1_PM_NSLOPE 33u     /* 32 x uint32: natural slopes, Q16 |
| `P1_PM_ORDER` | macro | `engine/gen/type_p_alpha_dsp.c:11438` | `#define P1_PM_ORDER 97u /* 32 x uint32: one-based y-span order */` | #define P1_PM_NFLIP 65u      /* 32 x uint32: natural segment orientation |
| `P1_PM_PY0` | macro | `engine/gen/type_p_alpha_dsp.c:11439` | `#define P1_PM_PY0 129u /* 32 x uint32: composed y at the input boundary */` | #define P1_PM_ORDER 97u      /* 32 x uint32: one-based y-span order |
| `P1_PM_PSLOPE` | macro | `engine/gen/type_p_alpha_dsp.c:11440` | `#define P1_PM_PSLOPE 161u /* 32 x uint32: composed slope, Q16 */` | #define P1_PM_PY0 129u       /* 32 x uint32: composed y at the input boundary |
| `P1_PM_SEG` | macro | `engine/gen/type_p_alpha_dsp.c:11441` | `#define P1_PM_SEG 193u /* 1024 x uint32: 4 packed segment indices per word */` | #define P1_PM_PSLOPE 161u    /* 32 x uint32: composed slope, Q16 |
| `P1_PM_F` | macro | `engine/gen/type_p_alpha_dsp.c:11442` | `#define P1_PM_F 1217u /* 1024 x uint32: F in 10-bit units (0..1024) */` | #define P1_PM_SEG 193u       /* 1024 x uint32: 4 packed segment indices per word |
| `P1_PM_LASTB` | macro | `engine/gen/type_p_alpha_dsp.c:11443` | `#define P1_PM_LASTB 2241u` | #define P1_PM_F 1217u        /* 1024 x uint32: F in 10-bit units (0..1024) |
| `P1_PM_SEED` | macro | `engine/gen/type_p_alpha_dsp.c:11444` | `#define P1_PM_SEED 2242u` |  |
| `P1_PM_SEEDED` | macro | `engine/gen/type_p_alpha_dsp.c:11445` | `#define P1_PM_SEEDED 2243u` |  |
| `P1_PM_BP` | macro | `engine/gen/type_p_alpha_dsp.c:11446` | `#define P1_PM_BP 2244u` |  |
| `P1_PM_LASTSEGS` | macro | `engine/gen/type_p_alpha_dsp.c:11447` | `#define P1_PM_LASTSEGS 2245u` |  |
| `P1_PM_DACC` | macro | `engine/gen/type_p_alpha_dsp.c:11448` | `#define P1_PM_DACC 2246u` |  |
| `P1_PM_FACC` | macro | `engine/gen/type_p_alpha_dsp.c:11449` | `#define P1_PM_FACC 2247u` |  |
| `P1_PM_RACC` | macro | `engine/gen/type_p_alpha_dsp.c:11450` | `#define P1_PM_RACC 2248u` |  |
| `P1_PM_NY` | macro | `engine/gen/type_p_alpha_dsp.c:11451` | `#define P1_PM_NY 2249u /* 33 x uint32: snapped y boundaries 0..4096 */` |  |
| `P1_PM_MASK` | macro | `engine/gen/type_p_alpha_dsp.c:11452` | `#define P1_PM_MASK 2282u /* 128 x uint32: 4096 protected-point bits */` | #define P1_PM_NY 2249u       /* 33 x uint32: snapped y boundaries 0..4096 |
| `P1_PM_MASKID` | macro | `engine/gen/type_p_alpha_dsp.c:11453` | `#define P1_PM_MASKID 2410u` | #define P1_PM_MASK 2282u     /* 128 x uint32: 4096 protected-point bits |
| `P1_PM_LOCK` | macro | `engine/gen/type_p_alpha_dsp.c:11454` | `#define P1_PM_LOCK 2411u /* bit j = y span j is locked */` |  |
| `P1_PM_ACTIVE` | macro | `engine/gen/type_p_alpha_dsp.c:11455` | `#define P1_PM_ACTIVE 2412u` | #define P1_PM_LOCK 2411u     /* bit j = y span j is locked |
| `P1_PM_LASTWAVE` | macro | `engine/gen/type_p_alpha_dsp.c:11456` | `#define P1_PM_LASTWAVE 2413u` |  |
| `P1_PM_LASTON` | macro | `engine/gen/type_p_alpha_dsp.c:11457` | `#define P1_PM_LASTON 2414u` |  |
| `P1_PHASE_READ` | macro | `engine/gen/type_p_alpha_dsp.c:11463` | `#define P1_PHASE_READ 0` |  |
| `P1_PM_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:11465` | `#define P1_PM_TABLE 2560u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:11467` | `#define P1_PM_TABLE_N 4096u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:11468` | `#define P1_PM_TABLE_STEP 1u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:11469` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:11471` | `#define P1_PM_TABLE_N 512u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:11472` | `#define P1_PM_TABLE_STEP 8u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:11473` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:11475` | `#define P1_PM_TABLE_N 512u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:11476` | `#define P1_PM_TABLE_STEP 8u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:11477` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:11479` | `#define P1_PM_TABLE_N 1024u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:11480` | `#define P1_PM_TABLE_STEP 4u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:11481` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:11483` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:11484` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:11485` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:11487` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:11488` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:11489` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:11491` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:11492` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:11493` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:11495` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:11496` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:11497` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:11499` | `#define P1_PM_TABLE_N 512u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:11500` | `#define P1_PM_TABLE_STEP 8u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:11501` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:11503` | `#define P1_PM_TABLE_N 1024u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:11504` | `#define P1_PM_TABLE_STEP 4u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:11505` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:11507` | `#define P1_PM_TABLE_N 1024u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:11508` | `#define P1_PM_TABLE_STEP 4u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:11509` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:11511` | `#define P1_PM_TABLE_N 0u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:11512` | `#define P1_PM_TABLE_STEP 1u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:11513` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:11516` | `#define P1_PM_TABLE_WORDS (((P1_PM_TABLE_N + P1_PM_TABLE_EXTRA) + 1u) / 2u)` |  |
| `P1_PM_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:11517` | `#define P1_PM_WORDS (P1_PM_TABLE + P1_PM_TABLE_WORDS)` |  |
| `P1_PM_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:11519` | `#define P1_PM_WORDS 2560u` |  |
| `P1_PM_TABLE_Q` | macro | `engine/gen/type_p_alpha_dsp.c:11522` | `#define P1_PM_TABLE_Q 1` |  |
| `P1_PM_TABLE_Q` | macro | `engine/gen/type_p_alpha_dsp.c:11524` | `#define P1_PM_TABLE_Q 0` |  |
| `P1_PM_SAMPLE` | macro | `engine/gen/type_p_alpha_dsp.c:11531` | `#define P1_PM_SAMPLE(wave, bank, part, q, ab) \ (((wave) >= 14u) \ ? (int32_t)((const int16_t *)(bank))[ \ ((ab) ? P1...` |  |
| `P1_PM_SET` | macro | `engine/gen/type_p_alpha_dsp.c:11540` | `#define P1_PM_SET(m, i) ((m)[(i) >> 5] \|= 1u << ((i) & 31u))` |  |
| `P1_PM_GET` | macro | `engine/gen/type_p_alpha_dsp.c:11541` | `#define P1_PM_GET(m, i) (((m)[(i) >> 5] >> ((i) & 31u)) & 1u)` |  |
| `P1_PM_MASK_BUILD` | macro | `engine/gen/type_p_alpha_dsp.c:11549` | `#define P1_PM_MASK_BUILD(dst, wave, bank, part, ab) do { \ for (uint32_t z = 0u; z < 128u; z++) (dst)[z] = 0u; \ int3...` |  |
| `P1_PHASE_MAP_LAYOUT` | macro | `engine/gen/type_p_alpha_dsp.c:11637` | `#define P1_PHASE_MAP_LAYOUT(pm, m) do { \ uint32_t pos = 0u; \ for (uint32_t k = 0u; k < (m); k++) { \ uint32_t j = (...` |  |
| `P1_PHASE_MAP_F` | macro | `engine/gen/type_p_alpha_dsp.c:11668` | `#define P1_PHASE_MAP_F(pm, xb) do { \ uint32_t xb12 = (uint32_t)(((xb) * 4096u + 500u) / 1000u); \ if (xb12 < 1u) xb1...` |  |
| `P1_PHASE_MAP_NEXT` | macro | `engine/gen/type_p_alpha_dsp.c:11684` | `#define P1_PHASE_MAP_NEXT(pm) \ ((pm)[P1_PM_SEED] ^= (pm)[P1_PM_SEED] << 13, \ (pm)[P1_PM_SEED] ^= (pm)[P1_PM_SEED] >...` |  |
| `P1_PHASE_MAP_Y` | macro | `engine/gen/type_p_alpha_dsp.c:11690` | `#define P1_PHASE_MAP_Y(pm, x, m, y) do { \ uint32_t yx = (x); \ uint32_t yq = (pm)[P1_PM_F + (yx >> 2)] << 2; \ if (y...` |  |
| `P1_PHASE_MAP_YQ` | macro | `engine/gen/type_p_alpha_dsp.c:11703` | `#define P1_PHASE_MAP_YQ(pm, q, m, y) do { \ uint32_t yq = (q); \ if (yq > 4095u) yq = 4095u; \ uint32_t ysh = (yq & 3...` | The post-F segment map y(q). It does not depend on the F breakpoint. |
| `P1_PHASE_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:11715` | `#define P1_PHASE_TABLE(pm, m) do { \ uint16_t *pt = (uint16_t *)(void *)((pm) + P1_PM_TABLE); \ for (uint32_t ti = 0u...` |  |
| `P1_PHASE_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:11729` | `#define P1_PHASE_TABLE(pm, m) do { \ uint16_t *pt = (uint16_t *)(void *)((pm) + P1_PM_TABLE); \ for (uint32_t ti = 0u...` |  |
| `P1_PHASE_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:11743` | `#define P1_PHASE_TABLE(pm, m) do { } while (0)` |  |
| `P1_PM_NX` | macro | `engine/gen/type_p_alpha_dsp.c:12231` | `#define P1_PM_NX 0u /* 33 x uint32: natural input boundaries 0..4096 */` | The pool starts at 2,560 words per part. The optional direct-read table is appended per part, so P1_PM_WORDS is defined after the table mode below. |
| `P1_PM_NSLOPE` | macro | `engine/gen/type_p_alpha_dsp.c:12232` | `#define P1_PM_NSLOPE 33u /* 32 x uint32: natural slopes, Q16 */` | #define P1_PM_NX 0u          /* 33 x uint32: natural input boundaries 0..4096 |
| `P1_PM_NFLIP` | macro | `engine/gen/type_p_alpha_dsp.c:12233` | `#define P1_PM_NFLIP 65u /* 32 x uint32: natural segment orientation */` | #define P1_PM_NSLOPE 33u     /* 32 x uint32: natural slopes, Q16 |
| `P1_PM_ORDER` | macro | `engine/gen/type_p_alpha_dsp.c:12234` | `#define P1_PM_ORDER 97u /* 32 x uint32: one-based y-span order */` | #define P1_PM_NFLIP 65u      /* 32 x uint32: natural segment orientation |
| `P1_PM_PY0` | macro | `engine/gen/type_p_alpha_dsp.c:12235` | `#define P1_PM_PY0 129u /* 32 x uint32: composed y at the input boundary */` | #define P1_PM_ORDER 97u      /* 32 x uint32: one-based y-span order |
| `P1_PM_PSLOPE` | macro | `engine/gen/type_p_alpha_dsp.c:12236` | `#define P1_PM_PSLOPE 161u /* 32 x uint32: composed slope, Q16 */` | #define P1_PM_PY0 129u       /* 32 x uint32: composed y at the input boundary |
| `P1_PM_SEG` | macro | `engine/gen/type_p_alpha_dsp.c:12237` | `#define P1_PM_SEG 193u /* 1024 x uint32: 4 packed segment indices per word */` | #define P1_PM_PSLOPE 161u    /* 32 x uint32: composed slope, Q16 |
| `P1_PM_F` | macro | `engine/gen/type_p_alpha_dsp.c:12238` | `#define P1_PM_F 1217u /* 1024 x uint32: F in 10-bit units (0..1024) */` | #define P1_PM_SEG 193u       /* 1024 x uint32: 4 packed segment indices per word |
| `P1_PM_LASTB` | macro | `engine/gen/type_p_alpha_dsp.c:12239` | `#define P1_PM_LASTB 2241u` | #define P1_PM_F 1217u        /* 1024 x uint32: F in 10-bit units (0..1024) |
| `P1_PM_SEED` | macro | `engine/gen/type_p_alpha_dsp.c:12240` | `#define P1_PM_SEED 2242u` |  |
| `P1_PM_SEEDED` | macro | `engine/gen/type_p_alpha_dsp.c:12241` | `#define P1_PM_SEEDED 2243u` |  |
| `P1_PM_BP` | macro | `engine/gen/type_p_alpha_dsp.c:12242` | `#define P1_PM_BP 2244u` |  |
| `P1_PM_LASTSEGS` | macro | `engine/gen/type_p_alpha_dsp.c:12243` | `#define P1_PM_LASTSEGS 2245u` |  |
| `P1_PM_DACC` | macro | `engine/gen/type_p_alpha_dsp.c:12244` | `#define P1_PM_DACC 2246u` |  |
| `P1_PM_FACC` | macro | `engine/gen/type_p_alpha_dsp.c:12245` | `#define P1_PM_FACC 2247u` |  |
| `P1_PM_RACC` | macro | `engine/gen/type_p_alpha_dsp.c:12246` | `#define P1_PM_RACC 2248u` |  |
| `P1_PM_NY` | macro | `engine/gen/type_p_alpha_dsp.c:12247` | `#define P1_PM_NY 2249u /* 33 x uint32: snapped y boundaries 0..4096 */` |  |
| `P1_PM_MASK` | macro | `engine/gen/type_p_alpha_dsp.c:12248` | `#define P1_PM_MASK 2282u /* 128 x uint32: 4096 protected-point bits */` | #define P1_PM_NY 2249u       /* 33 x uint32: snapped y boundaries 0..4096 |
| `P1_PM_MASKID` | macro | `engine/gen/type_p_alpha_dsp.c:12249` | `#define P1_PM_MASKID 2410u` | #define P1_PM_MASK 2282u     /* 128 x uint32: 4096 protected-point bits |
| `P1_PM_LOCK` | macro | `engine/gen/type_p_alpha_dsp.c:12250` | `#define P1_PM_LOCK 2411u /* bit j = y span j is locked */` |  |
| `P1_PM_ACTIVE` | macro | `engine/gen/type_p_alpha_dsp.c:12251` | `#define P1_PM_ACTIVE 2412u` | #define P1_PM_LOCK 2411u     /* bit j = y span j is locked |
| `P1_PM_LASTWAVE` | macro | `engine/gen/type_p_alpha_dsp.c:12252` | `#define P1_PM_LASTWAVE 2413u` |  |
| `P1_PM_LASTON` | macro | `engine/gen/type_p_alpha_dsp.c:12253` | `#define P1_PM_LASTON 2414u` |  |
| `P1_PHASE_READ` | macro | `engine/gen/type_p_alpha_dsp.c:12259` | `#define P1_PHASE_READ 0` |  |
| `P1_PM_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:12261` | `#define P1_PM_TABLE 2560u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:12263` | `#define P1_PM_TABLE_N 4096u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:12264` | `#define P1_PM_TABLE_STEP 1u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:12265` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:12267` | `#define P1_PM_TABLE_N 512u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:12268` | `#define P1_PM_TABLE_STEP 8u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:12269` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:12271` | `#define P1_PM_TABLE_N 512u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:12272` | `#define P1_PM_TABLE_STEP 8u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:12273` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:12275` | `#define P1_PM_TABLE_N 1024u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:12276` | `#define P1_PM_TABLE_STEP 4u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:12277` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:12279` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:12280` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:12281` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:12283` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:12284` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:12285` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:12287` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:12288` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:12289` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:12291` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:12292` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:12293` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:12295` | `#define P1_PM_TABLE_N 512u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:12296` | `#define P1_PM_TABLE_STEP 8u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:12297` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:12299` | `#define P1_PM_TABLE_N 1024u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:12300` | `#define P1_PM_TABLE_STEP 4u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:12301` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:12303` | `#define P1_PM_TABLE_N 1024u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:12304` | `#define P1_PM_TABLE_STEP 4u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:12305` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:12307` | `#define P1_PM_TABLE_N 0u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:12308` | `#define P1_PM_TABLE_STEP 1u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:12309` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:12312` | `#define P1_PM_TABLE_WORDS (((P1_PM_TABLE_N + P1_PM_TABLE_EXTRA) + 1u) / 2u)` |  |
| `P1_PM_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:12313` | `#define P1_PM_WORDS (P1_PM_TABLE + P1_PM_TABLE_WORDS)` |  |
| `P1_PM_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:12315` | `#define P1_PM_WORDS 2560u` |  |
| `P1_PM_TABLE_Q` | macro | `engine/gen/type_p_alpha_dsp.c:12318` | `#define P1_PM_TABLE_Q 1` |  |
| `P1_PM_TABLE_Q` | macro | `engine/gen/type_p_alpha_dsp.c:12320` | `#define P1_PM_TABLE_Q 0` |  |
| `P1_PM_SAMPLE` | macro | `engine/gen/type_p_alpha_dsp.c:12327` | `#define P1_PM_SAMPLE(wave, bank, part, q, ab) \ (((wave) >= 14u) \ ? (int32_t)((const int16_t *)(bank))[ \ ((ab) ? P1...` |  |
| `P1_PM_SET` | macro | `engine/gen/type_p_alpha_dsp.c:12336` | `#define P1_PM_SET(m, i) ((m)[(i) >> 5] \|= 1u << ((i) & 31u))` |  |
| `P1_PM_GET` | macro | `engine/gen/type_p_alpha_dsp.c:12337` | `#define P1_PM_GET(m, i) (((m)[(i) >> 5] >> ((i) & 31u)) & 1u)` |  |
| `P1_PM_MASK_BUILD` | macro | `engine/gen/type_p_alpha_dsp.c:12345` | `#define P1_PM_MASK_BUILD(dst, wave, bank, part, ab) do { \ for (uint32_t z = 0u; z < 128u; z++) (dst)[z] = 0u; \ int3...` |  |
| `P1_PHASE_MAP_LAYOUT` | macro | `engine/gen/type_p_alpha_dsp.c:12433` | `#define P1_PHASE_MAP_LAYOUT(pm, m) do { \ uint32_t pos = 0u; \ for (uint32_t k = 0u; k < (m); k++) { \ uint32_t j = (...` |  |
| `P1_PHASE_MAP_F` | macro | `engine/gen/type_p_alpha_dsp.c:12464` | `#define P1_PHASE_MAP_F(pm, xb) do { \ uint32_t xb12 = (uint32_t)(((xb) * 4096u + 500u) / 1000u); \ if (xb12 < 1u) xb1...` |  |
| `P1_PHASE_MAP_NEXT` | macro | `engine/gen/type_p_alpha_dsp.c:12480` | `#define P1_PHASE_MAP_NEXT(pm) \ ((pm)[P1_PM_SEED] ^= (pm)[P1_PM_SEED] << 13, \ (pm)[P1_PM_SEED] ^= (pm)[P1_PM_SEED] >...` |  |
| `P1_PHASE_MAP_Y` | macro | `engine/gen/type_p_alpha_dsp.c:12486` | `#define P1_PHASE_MAP_Y(pm, x, m, y) do { \ uint32_t yx = (x); \ uint32_t yq = (pm)[P1_PM_F + (yx >> 2)] << 2; \ if (y...` |  |
| `P1_PHASE_MAP_YQ` | macro | `engine/gen/type_p_alpha_dsp.c:12499` | `#define P1_PHASE_MAP_YQ(pm, q, m, y) do { \ uint32_t yq = (q); \ if (yq > 4095u) yq = 4095u; \ uint32_t ysh = (yq & 3...` | The post-F segment map y(q). It does not depend on the F breakpoint. |
| `P1_PHASE_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:12511` | `#define P1_PHASE_TABLE(pm, m) do { \ uint16_t *pt = (uint16_t *)(void *)((pm) + P1_PM_TABLE); \ for (uint32_t ti = 0u...` |  |
| `P1_PHASE_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:12525` | `#define P1_PHASE_TABLE(pm, m) do { \ uint16_t *pt = (uint16_t *)(void *)((pm) + P1_PM_TABLE); \ for (uint32_t ti = 0u...` |  |
| `P1_PHASE_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:12539` | `#define P1_PHASE_TABLE(pm, m) do { } while (0)` |  |
| `P1_PM_NX` | macro | `engine/gen/type_p_alpha_dsp.c:13027` | `#define P1_PM_NX 0u /* 33 x uint32: natural input boundaries 0..4096 */` | The pool starts at 2,560 words per part. The optional direct-read table is appended per part, so P1_PM_WORDS is defined after the table mode below. |
| `P1_PM_NSLOPE` | macro | `engine/gen/type_p_alpha_dsp.c:13028` | `#define P1_PM_NSLOPE 33u /* 32 x uint32: natural slopes, Q16 */` | #define P1_PM_NX 0u          /* 33 x uint32: natural input boundaries 0..4096 |
| `P1_PM_NFLIP` | macro | `engine/gen/type_p_alpha_dsp.c:13029` | `#define P1_PM_NFLIP 65u /* 32 x uint32: natural segment orientation */` | #define P1_PM_NSLOPE 33u     /* 32 x uint32: natural slopes, Q16 |
| `P1_PM_ORDER` | macro | `engine/gen/type_p_alpha_dsp.c:13030` | `#define P1_PM_ORDER 97u /* 32 x uint32: one-based y-span order */` | #define P1_PM_NFLIP 65u      /* 32 x uint32: natural segment orientation |
| `P1_PM_PY0` | macro | `engine/gen/type_p_alpha_dsp.c:13031` | `#define P1_PM_PY0 129u /* 32 x uint32: composed y at the input boundary */` | #define P1_PM_ORDER 97u      /* 32 x uint32: one-based y-span order |
| `P1_PM_PSLOPE` | macro | `engine/gen/type_p_alpha_dsp.c:13032` | `#define P1_PM_PSLOPE 161u /* 32 x uint32: composed slope, Q16 */` | #define P1_PM_PY0 129u       /* 32 x uint32: composed y at the input boundary |
| `P1_PM_SEG` | macro | `engine/gen/type_p_alpha_dsp.c:13033` | `#define P1_PM_SEG 193u /* 1024 x uint32: 4 packed segment indices per word */` | #define P1_PM_PSLOPE 161u    /* 32 x uint32: composed slope, Q16 |
| `P1_PM_F` | macro | `engine/gen/type_p_alpha_dsp.c:13034` | `#define P1_PM_F 1217u /* 1024 x uint32: F in 10-bit units (0..1024) */` | #define P1_PM_SEG 193u       /* 1024 x uint32: 4 packed segment indices per word |
| `P1_PM_LASTB` | macro | `engine/gen/type_p_alpha_dsp.c:13035` | `#define P1_PM_LASTB 2241u` | #define P1_PM_F 1217u        /* 1024 x uint32: F in 10-bit units (0..1024) |
| `P1_PM_SEED` | macro | `engine/gen/type_p_alpha_dsp.c:13036` | `#define P1_PM_SEED 2242u` |  |
| `P1_PM_SEEDED` | macro | `engine/gen/type_p_alpha_dsp.c:13037` | `#define P1_PM_SEEDED 2243u` |  |
| `P1_PM_BP` | macro | `engine/gen/type_p_alpha_dsp.c:13038` | `#define P1_PM_BP 2244u` |  |
| `P1_PM_LASTSEGS` | macro | `engine/gen/type_p_alpha_dsp.c:13039` | `#define P1_PM_LASTSEGS 2245u` |  |
| `P1_PM_DACC` | macro | `engine/gen/type_p_alpha_dsp.c:13040` | `#define P1_PM_DACC 2246u` |  |
| `P1_PM_FACC` | macro | `engine/gen/type_p_alpha_dsp.c:13041` | `#define P1_PM_FACC 2247u` |  |
| `P1_PM_RACC` | macro | `engine/gen/type_p_alpha_dsp.c:13042` | `#define P1_PM_RACC 2248u` |  |
| `P1_PM_NY` | macro | `engine/gen/type_p_alpha_dsp.c:13043` | `#define P1_PM_NY 2249u /* 33 x uint32: snapped y boundaries 0..4096 */` |  |
| `P1_PM_MASK` | macro | `engine/gen/type_p_alpha_dsp.c:13044` | `#define P1_PM_MASK 2282u /* 128 x uint32: 4096 protected-point bits */` | #define P1_PM_NY 2249u       /* 33 x uint32: snapped y boundaries 0..4096 |
| `P1_PM_MASKID` | macro | `engine/gen/type_p_alpha_dsp.c:13045` | `#define P1_PM_MASKID 2410u` | #define P1_PM_MASK 2282u     /* 128 x uint32: 4096 protected-point bits |
| `P1_PM_LOCK` | macro | `engine/gen/type_p_alpha_dsp.c:13046` | `#define P1_PM_LOCK 2411u /* bit j = y span j is locked */` |  |
| `P1_PM_ACTIVE` | macro | `engine/gen/type_p_alpha_dsp.c:13047` | `#define P1_PM_ACTIVE 2412u` | #define P1_PM_LOCK 2411u     /* bit j = y span j is locked |
| `P1_PM_LASTWAVE` | macro | `engine/gen/type_p_alpha_dsp.c:13048` | `#define P1_PM_LASTWAVE 2413u` |  |
| `P1_PM_LASTON` | macro | `engine/gen/type_p_alpha_dsp.c:13049` | `#define P1_PM_LASTON 2414u` |  |
| `P1_PHASE_READ` | macro | `engine/gen/type_p_alpha_dsp.c:13055` | `#define P1_PHASE_READ 0` |  |
| `P1_PM_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:13057` | `#define P1_PM_TABLE 2560u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13059` | `#define P1_PM_TABLE_N 4096u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13060` | `#define P1_PM_TABLE_STEP 1u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13061` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13063` | `#define P1_PM_TABLE_N 512u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13064` | `#define P1_PM_TABLE_STEP 8u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13065` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13067` | `#define P1_PM_TABLE_N 512u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13068` | `#define P1_PM_TABLE_STEP 8u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13069` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13071` | `#define P1_PM_TABLE_N 1024u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13072` | `#define P1_PM_TABLE_STEP 4u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13073` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13075` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13076` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13077` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13079` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13080` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13081` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13083` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13084` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13085` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13087` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13088` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13089` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13091` | `#define P1_PM_TABLE_N 512u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13092` | `#define P1_PM_TABLE_STEP 8u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13093` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13095` | `#define P1_PM_TABLE_N 1024u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13096` | `#define P1_PM_TABLE_STEP 4u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13097` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13099` | `#define P1_PM_TABLE_N 1024u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13100` | `#define P1_PM_TABLE_STEP 4u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13101` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13103` | `#define P1_PM_TABLE_N 0u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13104` | `#define P1_PM_TABLE_STEP 1u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13105` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:13108` | `#define P1_PM_TABLE_WORDS (((P1_PM_TABLE_N + P1_PM_TABLE_EXTRA) + 1u) / 2u)` |  |
| `P1_PM_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:13109` | `#define P1_PM_WORDS (P1_PM_TABLE + P1_PM_TABLE_WORDS)` |  |
| `P1_PM_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:13111` | `#define P1_PM_WORDS 2560u` |  |
| `P1_PM_TABLE_Q` | macro | `engine/gen/type_p_alpha_dsp.c:13114` | `#define P1_PM_TABLE_Q 1` |  |
| `P1_PM_TABLE_Q` | macro | `engine/gen/type_p_alpha_dsp.c:13116` | `#define P1_PM_TABLE_Q 0` |  |
| `P1_PM_SAMPLE` | macro | `engine/gen/type_p_alpha_dsp.c:13123` | `#define P1_PM_SAMPLE(wave, bank, part, q, ab) \ (((wave) >= 14u) \ ? (int32_t)((const int16_t *)(bank))[ \ ((ab) ? P1...` |  |
| `P1_PM_SET` | macro | `engine/gen/type_p_alpha_dsp.c:13132` | `#define P1_PM_SET(m, i) ((m)[(i) >> 5] \|= 1u << ((i) & 31u))` |  |
| `P1_PM_GET` | macro | `engine/gen/type_p_alpha_dsp.c:13133` | `#define P1_PM_GET(m, i) (((m)[(i) >> 5] >> ((i) & 31u)) & 1u)` |  |
| `P1_PM_MASK_BUILD` | macro | `engine/gen/type_p_alpha_dsp.c:13141` | `#define P1_PM_MASK_BUILD(dst, wave, bank, part, ab) do { \ for (uint32_t z = 0u; z < 128u; z++) (dst)[z] = 0u; \ int3...` |  |
| `P1_PHASE_MAP_LAYOUT` | macro | `engine/gen/type_p_alpha_dsp.c:13229` | `#define P1_PHASE_MAP_LAYOUT(pm, m) do { \ uint32_t pos = 0u; \ for (uint32_t k = 0u; k < (m); k++) { \ uint32_t j = (...` |  |
| `P1_PHASE_MAP_F` | macro | `engine/gen/type_p_alpha_dsp.c:13260` | `#define P1_PHASE_MAP_F(pm, xb) do { \ uint32_t xb12 = (uint32_t)(((xb) * 4096u + 500u) / 1000u); \ if (xb12 < 1u) xb1...` |  |
| `P1_PHASE_MAP_NEXT` | macro | `engine/gen/type_p_alpha_dsp.c:13276` | `#define P1_PHASE_MAP_NEXT(pm) \ ((pm)[P1_PM_SEED] ^= (pm)[P1_PM_SEED] << 13, \ (pm)[P1_PM_SEED] ^= (pm)[P1_PM_SEED] >...` |  |
| `P1_PHASE_MAP_Y` | macro | `engine/gen/type_p_alpha_dsp.c:13282` | `#define P1_PHASE_MAP_Y(pm, x, m, y) do { \ uint32_t yx = (x); \ uint32_t yq = (pm)[P1_PM_F + (yx >> 2)] << 2; \ if (y...` |  |
| `P1_PHASE_MAP_YQ` | macro | `engine/gen/type_p_alpha_dsp.c:13295` | `#define P1_PHASE_MAP_YQ(pm, q, m, y) do { \ uint32_t yq = (q); \ if (yq > 4095u) yq = 4095u; \ uint32_t ysh = (yq & 3...` | The post-F segment map y(q). It does not depend on the F breakpoint. |
| `P1_PHASE_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:13307` | `#define P1_PHASE_TABLE(pm, m) do { \ uint16_t *pt = (uint16_t *)(void *)((pm) + P1_PM_TABLE); \ for (uint32_t ti = 0u...` |  |
| `P1_PHASE_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:13321` | `#define P1_PHASE_TABLE(pm, m) do { \ uint16_t *pt = (uint16_t *)(void *)((pm) + P1_PM_TABLE); \ for (uint32_t ti = 0u...` |  |
| `P1_PHASE_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:13335` | `#define P1_PHASE_TABLE(pm, m) do { } while (0)` |  |
| `P1_PM_NX` | macro | `engine/gen/type_p_alpha_dsp.c:13823` | `#define P1_PM_NX 0u /* 33 x uint32: natural input boundaries 0..4096 */` | The pool starts at 2,560 words per part. The optional direct-read table is appended per part, so P1_PM_WORDS is defined after the table mode below. |
| `P1_PM_NSLOPE` | macro | `engine/gen/type_p_alpha_dsp.c:13824` | `#define P1_PM_NSLOPE 33u /* 32 x uint32: natural slopes, Q16 */` | #define P1_PM_NX 0u          /* 33 x uint32: natural input boundaries 0..4096 |
| `P1_PM_NFLIP` | macro | `engine/gen/type_p_alpha_dsp.c:13825` | `#define P1_PM_NFLIP 65u /* 32 x uint32: natural segment orientation */` | #define P1_PM_NSLOPE 33u     /* 32 x uint32: natural slopes, Q16 |
| `P1_PM_ORDER` | macro | `engine/gen/type_p_alpha_dsp.c:13826` | `#define P1_PM_ORDER 97u /* 32 x uint32: one-based y-span order */` | #define P1_PM_NFLIP 65u      /* 32 x uint32: natural segment orientation |
| `P1_PM_PY0` | macro | `engine/gen/type_p_alpha_dsp.c:13827` | `#define P1_PM_PY0 129u /* 32 x uint32: composed y at the input boundary */` | #define P1_PM_ORDER 97u      /* 32 x uint32: one-based y-span order |
| `P1_PM_PSLOPE` | macro | `engine/gen/type_p_alpha_dsp.c:13828` | `#define P1_PM_PSLOPE 161u /* 32 x uint32: composed slope, Q16 */` | #define P1_PM_PY0 129u       /* 32 x uint32: composed y at the input boundary |
| `P1_PM_SEG` | macro | `engine/gen/type_p_alpha_dsp.c:13829` | `#define P1_PM_SEG 193u /* 1024 x uint32: 4 packed segment indices per word */` | #define P1_PM_PSLOPE 161u    /* 32 x uint32: composed slope, Q16 |
| `P1_PM_F` | macro | `engine/gen/type_p_alpha_dsp.c:13830` | `#define P1_PM_F 1217u /* 1024 x uint32: F in 10-bit units (0..1024) */` | #define P1_PM_SEG 193u       /* 1024 x uint32: 4 packed segment indices per word |
| `P1_PM_LASTB` | macro | `engine/gen/type_p_alpha_dsp.c:13831` | `#define P1_PM_LASTB 2241u` | #define P1_PM_F 1217u        /* 1024 x uint32: F in 10-bit units (0..1024) |
| `P1_PM_SEED` | macro | `engine/gen/type_p_alpha_dsp.c:13832` | `#define P1_PM_SEED 2242u` |  |
| `P1_PM_SEEDED` | macro | `engine/gen/type_p_alpha_dsp.c:13833` | `#define P1_PM_SEEDED 2243u` |  |
| `P1_PM_BP` | macro | `engine/gen/type_p_alpha_dsp.c:13834` | `#define P1_PM_BP 2244u` |  |
| `P1_PM_LASTSEGS` | macro | `engine/gen/type_p_alpha_dsp.c:13835` | `#define P1_PM_LASTSEGS 2245u` |  |
| `P1_PM_DACC` | macro | `engine/gen/type_p_alpha_dsp.c:13836` | `#define P1_PM_DACC 2246u` |  |
| `P1_PM_FACC` | macro | `engine/gen/type_p_alpha_dsp.c:13837` | `#define P1_PM_FACC 2247u` |  |
| `P1_PM_RACC` | macro | `engine/gen/type_p_alpha_dsp.c:13838` | `#define P1_PM_RACC 2248u` |  |
| `P1_PM_NY` | macro | `engine/gen/type_p_alpha_dsp.c:13839` | `#define P1_PM_NY 2249u /* 33 x uint32: snapped y boundaries 0..4096 */` |  |
| `P1_PM_MASK` | macro | `engine/gen/type_p_alpha_dsp.c:13840` | `#define P1_PM_MASK 2282u /* 128 x uint32: 4096 protected-point bits */` | #define P1_PM_NY 2249u       /* 33 x uint32: snapped y boundaries 0..4096 |
| `P1_PM_MASKID` | macro | `engine/gen/type_p_alpha_dsp.c:13841` | `#define P1_PM_MASKID 2410u` | #define P1_PM_MASK 2282u     /* 128 x uint32: 4096 protected-point bits |
| `P1_PM_LOCK` | macro | `engine/gen/type_p_alpha_dsp.c:13842` | `#define P1_PM_LOCK 2411u /* bit j = y span j is locked */` |  |
| `P1_PM_ACTIVE` | macro | `engine/gen/type_p_alpha_dsp.c:13843` | `#define P1_PM_ACTIVE 2412u` | #define P1_PM_LOCK 2411u     /* bit j = y span j is locked |
| `P1_PM_LASTWAVE` | macro | `engine/gen/type_p_alpha_dsp.c:13844` | `#define P1_PM_LASTWAVE 2413u` |  |
| `P1_PM_LASTON` | macro | `engine/gen/type_p_alpha_dsp.c:13845` | `#define P1_PM_LASTON 2414u` |  |
| `P1_PHASE_READ` | macro | `engine/gen/type_p_alpha_dsp.c:13851` | `#define P1_PHASE_READ 0` |  |
| `P1_PM_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:13853` | `#define P1_PM_TABLE 2560u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13855` | `#define P1_PM_TABLE_N 4096u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13856` | `#define P1_PM_TABLE_STEP 1u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13857` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13859` | `#define P1_PM_TABLE_N 512u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13860` | `#define P1_PM_TABLE_STEP 8u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13861` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13863` | `#define P1_PM_TABLE_N 512u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13864` | `#define P1_PM_TABLE_STEP 8u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13865` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13867` | `#define P1_PM_TABLE_N 1024u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13868` | `#define P1_PM_TABLE_STEP 4u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13869` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13871` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13872` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13873` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13875` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13876` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13877` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13879` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13880` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13881` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13883` | `#define P1_PM_TABLE_N 256u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13884` | `#define P1_PM_TABLE_STEP 16u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13885` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13887` | `#define P1_PM_TABLE_N 512u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13888` | `#define P1_PM_TABLE_STEP 8u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13889` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13891` | `#define P1_PM_TABLE_N 1024u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13892` | `#define P1_PM_TABLE_STEP 4u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13893` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13895` | `#define P1_PM_TABLE_N 1024u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13896` | `#define P1_PM_TABLE_STEP 4u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13897` | `#define P1_PM_TABLE_EXTRA 1u` |  |
| `P1_PM_TABLE_N` | macro | `engine/gen/type_p_alpha_dsp.c:13899` | `#define P1_PM_TABLE_N 0u` |  |
| `P1_PM_TABLE_STEP` | macro | `engine/gen/type_p_alpha_dsp.c:13900` | `#define P1_PM_TABLE_STEP 1u` |  |
| `P1_PM_TABLE_EXTRA` | macro | `engine/gen/type_p_alpha_dsp.c:13901` | `#define P1_PM_TABLE_EXTRA 0u` |  |
| `P1_PM_TABLE_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:13904` | `#define P1_PM_TABLE_WORDS (((P1_PM_TABLE_N + P1_PM_TABLE_EXTRA) + 1u) / 2u)` |  |
| `P1_PM_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:13905` | `#define P1_PM_WORDS (P1_PM_TABLE + P1_PM_TABLE_WORDS)` |  |
| `P1_PM_WORDS` | macro | `engine/gen/type_p_alpha_dsp.c:13907` | `#define P1_PM_WORDS 2560u` |  |
| `P1_PM_TABLE_Q` | macro | `engine/gen/type_p_alpha_dsp.c:13910` | `#define P1_PM_TABLE_Q 1` |  |
| `P1_PM_TABLE_Q` | macro | `engine/gen/type_p_alpha_dsp.c:13912` | `#define P1_PM_TABLE_Q 0` |  |
| `P1_PM_SAMPLE` | macro | `engine/gen/type_p_alpha_dsp.c:13919` | `#define P1_PM_SAMPLE(wave, bank, part, q, ab) \ (((wave) >= 14u) \ ? (int32_t)((const int16_t *)(bank))[ \ ((ab) ? P1...` |  |
| `P1_PM_SET` | macro | `engine/gen/type_p_alpha_dsp.c:13928` | `#define P1_PM_SET(m, i) ((m)[(i) >> 5] \|= 1u << ((i) & 31u))` |  |
| `P1_PM_GET` | macro | `engine/gen/type_p_alpha_dsp.c:13929` | `#define P1_PM_GET(m, i) (((m)[(i) >> 5] >> ((i) & 31u)) & 1u)` |  |
| `P1_PM_MASK_BUILD` | macro | `engine/gen/type_p_alpha_dsp.c:13937` | `#define P1_PM_MASK_BUILD(dst, wave, bank, part, ab) do { \ for (uint32_t z = 0u; z < 128u; z++) (dst)[z] = 0u; \ int3...` |  |
| `P1_PHASE_MAP_LAYOUT` | macro | `engine/gen/type_p_alpha_dsp.c:14025` | `#define P1_PHASE_MAP_LAYOUT(pm, m) do { \ uint32_t pos = 0u; \ for (uint32_t k = 0u; k < (m); k++) { \ uint32_t j = (...` |  |
| `P1_PHASE_MAP_F` | macro | `engine/gen/type_p_alpha_dsp.c:14056` | `#define P1_PHASE_MAP_F(pm, xb) do { \ uint32_t xb12 = (uint32_t)(((xb) * 4096u + 500u) / 1000u); \ if (xb12 < 1u) xb1...` |  |
| `P1_PHASE_MAP_NEXT` | macro | `engine/gen/type_p_alpha_dsp.c:14072` | `#define P1_PHASE_MAP_NEXT(pm) \ ((pm)[P1_PM_SEED] ^= (pm)[P1_PM_SEED] << 13, \ (pm)[P1_PM_SEED] ^= (pm)[P1_PM_SEED] >...` |  |
| `P1_PHASE_MAP_Y` | macro | `engine/gen/type_p_alpha_dsp.c:14078` | `#define P1_PHASE_MAP_Y(pm, x, m, y) do { \ uint32_t yx = (x); \ uint32_t yq = (pm)[P1_PM_F + (yx >> 2)] << 2; \ if (y...` |  |
| `P1_PHASE_MAP_YQ` | macro | `engine/gen/type_p_alpha_dsp.c:14091` | `#define P1_PHASE_MAP_YQ(pm, q, m, y) do { \ uint32_t yq = (q); \ if (yq > 4095u) yq = 4095u; \ uint32_t ysh = (yq & 3...` | The post-F segment map y(q). It does not depend on the F breakpoint. |
| `P1_PHASE_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:14103` | `#define P1_PHASE_TABLE(pm, m) do { \ uint16_t *pt = (uint16_t *)(void *)((pm) + P1_PM_TABLE); \ for (uint32_t ti = 0u...` |  |
| `P1_PHASE_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:14117` | `#define P1_PHASE_TABLE(pm, m) do { \ uint16_t *pt = (uint16_t *)(void *)((pm) + P1_PM_TABLE); \ for (uint32_t ti = 0u...` |  |
| `P1_PHASE_TABLE` | macro | `engine/gen/type_p_alpha_dsp.c:14131` | `#define P1_PHASE_TABLE(pm, m) do { } while (0)` |  |
| `p1_wave_harmonics` | global | `engine/gen/wave_harmonics.h:10` | `static const uint8_t p1_wave_harmonics[14][16] = { { 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, { 99, 0, 20, 0...` |  |
| `p1_wave` | global | `engine/gen/wavetable.h:15` | `static const int16_t p1_wave[47104] = { 0, 201, 402, 603, 804, 1005, 1206, 1407, 1608, 1809, 2009, 2210, 2411, 2611, ...` |  |
| `p1_wave_base` | global | `engine/gen/wavetable.h:2962` | `static const uint8_t p1_wave_base[14] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 19, 28, 37, }` |  |
| `P1_MATH_INLINE` | macro | `engine/hal/p1_math.h:17` | `#define P1_MATH_INLINE static inline __attribute__((always_inline))` | These helpers belong in the hot loop, so they are always inline. The plugin host inlines them at the call site. |
| `p1_fft_alloc` | macro | `engine/hal/p1_math.h:361` | `#define p1_fft_alloc(nfft, inverse, mem, lenmem) kiss_fft_alloc(nfft, inverse, mem, lenmem)` |  |
| `p1_fft` | macro | `engine/hal/p1_math.h:362` | `#define p1_fft(cfg, in, out) kiss_fft(cfg, in, out)` |  |
| `p1_fft_free` | macro | `engine/hal/p1_math.h:363` | `#define p1_fft_free kiss_fft_free` |  |
| `p1_fft_next_fast_size` | macro | `engine/hal/p1_math.h:364` | `#define p1_fft_next_fast_size kiss_fft_next_fast_size` |  |
| `P1_SPEC_PARTS` | macro | `engine/hal/spectral.h:21` | `#define P1_SPEC_PARTS 2` |  |
| `P1_SPEC_HARMONICS` | macro | `engine/hal/spectral.h:22` | `#define P1_SPEC_HARMONICS 64` |  |
| `P1_SPEC_SIZE` | macro | `engine/hal/spectral.h:23` | `#define P1_SPEC_SIZE 256` |  |
| `P1_SPEC_LEVELS` | macro | `engine/hal/spectral.h:24` | `#define P1_SPEC_LEVELS 7` |  |
| `P1_SPEC_TABLE` | macro | `engine/hal/spectral.h:25` | `#define P1_SPEC_TABLE (P1_SPEC_LEVELS * P1_SPEC_SIZE)` |  |
| `P1_SPEC_HEADER` | macro | `engine/hal/spectral.h:26` | `#define P1_SPEC_HEADER 128` |  |
| `P1_SPEC_BANK` | macro | `engine/hal/spectral.h:27` | `#define P1_SPEC_BANK 10880` |  |
| `P1_SPEC_A` | macro | `engine/hal/spectral.h:29` | `#define P1_SPEC_A(p) (P1_SPEC_HEADER + (p) * P1_SPEC_TABLE)` |  |
| `P1_SPEC_B` | macro | `engine/hal/spectral.h:30` | `#define P1_SPEC_B(p) \ (P1_SPEC_HEADER + P1_SPEC_PARTS * P1_SPEC_TABLE + (p) * P1_SPEC_TABLE)` |  |
| `P1_SPEC_SHADOW` | macro | `engine/hal/spectral.h:32` | `#define P1_SPEC_SHADOW(p) \ (P1_SPEC_HEADER + 2 * P1_SPEC_PARTS * P1_SPEC_TABLE + \ (p) * P1_SPEC_TABLE)` |  |
| `P1_SPEC_XFADE` | macro | `engine/hal/spectral.h:36` | `#define P1_SPEC_XFADE 0` |  |
| `P1_SPEC_TARGET` | macro | `engine/hal/spectral.h:37` | `#define P1_SPEC_TARGET 2` |  |
| `P1_SPEC_HASH` | macro | `engine/hal/spectral.h:38` | `#define P1_SPEC_HASH 4` |  |
| `P1_SPEC_TARGET_HASH` | macro | `engine/hal/spectral.h:39` | `#define P1_SPEC_TARGET_HASH 8` |  |
| `P1_SPEC_USED` | macro | `engine/hal/spectral.h:40` | `#define P1_SPEC_USED 12` |  |
| `table_routing` | global | `engine/runtime/macro_apply.h:13` | `extern const int16_t table_routing[200]` |  |
| `PART_BANK_SLOT` | macro | `engine/runtime/presets.c:240` | `#define PART_BANK_SLOT 0u /* the part bank starts at flash slot 0, the multi bank follows */` |  |
| `PRESET_NAME_LEN` | macro | `engine/runtime/presets.h:14` | `#define PRESET_NAME_LEN 33u` |  |
| `PRESET_VERSION` | macro | `engine/runtime/presets.h:15` | `#define PRESET_VERSION 19u` |  |
| `PRESET_CUTOFF_LAW_VERSION` | macro | `engine/runtime/presets.h:18` | `#define PRESET_CUTOFF_LAW_VERSION 7u` | The version that introduced the current cutoff law. A record older than this needs `cutoffOldToNew`. Version 7 and later already carry the new position. |
| `PRESET_EMPTY` | macro | `engine/runtime/presets.h:19` | `#define PRESET_EMPTY 0xFFu` |  |
| `PRESET_PART_SLOTS` | macro | `engine/runtime/presets.h:25` | `#define PRESET_PART_SLOTS 128u` | The logical slot counts: 128 single-part slots + 64 multi-patch slots. Each bank is a contiguous byte array chunked across storage slots (a record never straddles a slot boundary). `PRESET_SLOT_MAX` is the storage slot byte ceiling, mirrored here so the host build can compile presets.c. The part record is sized from the part parameter count, and the multi record fits all 64 slots in one. |
| `PRESET_MULTI_SLOTS` | macro | `engine/runtime/presets.h:26` | `#define PRESET_MULTI_SLOTS 64u` |  |
| `PRESET_SLOT_MAX` | macro | `engine/runtime/presets.h:27` | `#define PRESET_SLOT_MAX 16372u` |  |
| `SYNTH_PARAM_MAX` | macro | `engine/runtime/presets.h:31` | `#define SYNTH_PARAM_MAX 256u` | The per-part timbre param cap: the preset apply buffers are sized to this, so a synth's `SYNTH_N_PARAMS` must stay under it. |
| `PP_RECORD` | macro | `engine/runtime/presets.h:33` | `#define PP_RECORD(N) (1u + PRESET_NAME_LEN + (N) * 4u + 4u)` |  |
| `PRESET_MACRO_COUNT` | macro | `engine/runtime/presets.h:37` | `#define PRESET_MACRO_COUNT 6u` | The six patch macros: 54 int32 values and six name fields, appended to the multi record. The old record size stays available so a library written before the macro block still loads. |
| `PRESET_MACRO_PARAMS` | macro | `engine/runtime/presets.h:38` | `#define PRESET_MACRO_PARAMS 54u` |  |
| `PRESET_MACRO_BYTES` | macro | `engine/runtime/presets.h:39` | `#define PRESET_MACRO_BYTES (PRESET_MACRO_PARAMS * 4u + PRESET_MACRO_COUNT * PRESET_NAME_LEN)` |  |
| `MP_RECORD` | macro | `engine/runtime/presets.h:40` | `#define MP_RECORD(P) (1u + PRESET_NAME_LEN + (P) + (P) * 5u * 4u + PRESET_MACRO_BYTES + 4u)` |  |
| `MP_RECORD_OLD` | macro | `engine/runtime/presets.h:41` | `#define MP_RECORD_OLD(P) (1u + PRESET_NAME_LEN + (P) + (P) * 5u * 4u + 4u)` |  |
| `ST_RECORD` | macro | `engine/runtime/presets.h:42` | `#define ST_RECORD (1u + 4u + 4u + 4u)` |  |
| `PP_BANK` | macro | `engine/runtime/presets.h:43` | `#define PP_BANK(N) (PRESET_PART_SLOTS * PP_RECORD(N))` |  |
| `MP_BANK` | macro | `engine/runtime/presets.h:44` | `#define MP_BANK(P) (PRESET_MULTI_SLOTS * MP_RECORD(P))` |  |
| `SYNTH_CC_NONE` | macro | `engine/runtime/synth_midi.h:18` | `#define SYNTH_CC_NONE 0xFFFFu` |  |
| `ROOT` | module constant | `manual/build.py:19` | `ROOT = Path(__file__).resolve().parents[1]` |  |
| `MANUAL` | module constant | `manual/build.py:20` | `MANUAL = ROOT / "manual"` |  |
| `OUT` | module constant | `manual/build.py:21` | `OUT = ROOT / "build" / "manual"` |  |
| `BROWSERS` | module constant | `manual/build.py:23` | `BROWSERS = ( Path(r"C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe"), Path(r"C:\Program Files\Microsoft...` |  |
| `COVER` | module constant | `manual/build.py:33` | `COVER = """<section class="cover"> <div class="cover-mark">TYPE P.<span class="dot">.</span><span class="alpha">α</sp...` |  |
| `COLOPHON` | module constant | `manual/build.py:41` | `COLOPHON = """<div class="colophon"> <h2>Colophon</h2> <p>{title}, draft, {date}.</p> <p>Set in Zen Dots, Libre Frank...` |  |
| `toc` | module constant | `manual/build.py:51` | `toc = toc.strip()` |  |
| `toc` | module constant | `manual/build.py:53` | `toc = toc[len('<div class="toc">') :]` |  |
| `toc` | module constant | `manual/build.py:55` | `toc = toc[: -len("</div>")]` |  |
| `manifest` | module constant | `manual/build.py:64` | `manifest = json.loads((MANUAL / "manifest.json").read_text(encoding="utf-8"))` |  |
| `md` | module constant | `manual/build.py:67` | `md = markdown.Markdown( extensions=["toc", "tables", "fenced_code", "attr_list", "md_in_html"] )` |  |
| `parts` | module constant | `manual/build.py:70` | `parts: list[str] = []` |  |
| `source` | module constant | `manual/build.py:72` | `source = MANUAL / "chapters" / chapter["file"]` |  |
| `content` | module constant | `manual/build.py:74` | `content = "\n".join(parts)` |  |
| `toc` | module constant | `manual/build.py:75` | `toc = strip_toc(md.toc)` |  |
| `date` | module constant | `manual/build.py:77` | `date = datetime.date.today().isoformat()` |  |
| `template` | module constant | `manual/build.py:78` | `template = (MANUAL / "theme" / "template.html").read_text(encoding="utf-8")` |  |
| `common` | module constant | `manual/build.py:79` | `common = { "{{title}}": manifest["title"], "{{doc_kind}}": manifest["doc_kind"], "{{date}}": date, "{{toc}}": toc, "{...` |  |
| `screen` | module constant | `manual/build.py:89` | `screen = template` |  |
| `screen` | module constant | `manual/build.py:91` | `screen = screen.replace(key, value)` |  |
| `screen` | module constant | `manual/build.py:92` | `screen = ( screen.replace("{{css}}", "screen.css") .replace("{{body_class}}", "screen") .replace("{{cover}}", "") )` |  |
| `print_page` | module constant | `manual/build.py:99` | `print_page = template` |  |
| `print_page` | module constant | `manual/build.py:101` | `print_page = print_page.replace(key, value)` |  |
| `print_page` | module constant | `manual/build.py:102` | `print_page = ( print_page.replace("{{css}}", "print.css") .replace("{{body_class}}", "print") .replace( "{{cover}}", ...` |  |
| `browser` | module constant | `manual/build.py:118` | `browser = find_browser()` |  |
| `pdf` | module constant | `manual/build.py:119` | `pdf = OUT / "manual.pdf"` |  |
| `result` | module constant | `manual/build.py:123` | `result = subprocess.run( [ str(browser), "--headless=new", "--disable-gpu", "--no-pdf-header-footer", f"--print-to-pd...` |  |
| `ROOT` | module constant | `manual/figures.py:8` | `ROOT = Path(__file__).resolve().parents[1]` |  |
| `SHOTS` | module constant | `manual/figures.py:9` | `SHOTS = ROOT / "plugin" / "tests" / "snapshots" / "linux-x86_64"` |  |
| `ENVELOPE` | module constant | `manual/figures.py:11` | `ENVELOPE = """<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 720 260" role="img" aria-labelledby="amp-title"> <...` |  |
| `figures` | module constant | `manual/figures.py:39` | `figures = out / "figures"` |  |
| `source` | module constant | `manual/figures.py:42` | `source = SHOTS / name` |  |
| `kPmWords` | global | `plugin/engine/Engine.cpp:15` | `constexpr uint32_t kPmWords = phase_map::kWords` | The phase_map pool layout. These offsets are the node's layout. The pool stride comes from the shared header so a read-table build keeps the parts apart. |
| `kPmNx` | global | `plugin/engine/Engine.cpp:16` | `constexpr uint32_t kPmNx = 0u` |  |
| `kPmNflip` | global | `plugin/engine/Engine.cpp:17` | `constexpr uint32_t kPmNflip = 65u` |  |
| `kPmOrder` | global | `plugin/engine/Engine.cpp:18` | `constexpr uint32_t kPmOrder = 97u` |  |
| `kPmPy0` | global | `plugin/engine/Engine.cpp:19` | `constexpr uint32_t kPmPy0 = 129u` |  |
| `kPmPslope` | global | `plugin/engine/Engine.cpp:20` | `constexpr uint32_t kPmPslope = 161u` |  |
| `kPmSeg` | global | `plugin/engine/Engine.cpp:21` | `constexpr uint32_t kPmSeg = 193u` |  |
| `kPmF` | global | `plugin/engine/Engine.cpp:22` | `constexpr uint32_t kPmF = 1217u` |  |
| `kPmSeed` | global | `plugin/engine/Engine.cpp:23` | `constexpr uint32_t kPmSeed = 2242u` |  |
| `kPmSeeded` | global | `plugin/engine/Engine.cpp:24` | `constexpr uint32_t kPmSeeded = 2243u` |  |
| `kPmBp` | global | `plugin/engine/Engine.cpp:25` | `constexpr uint32_t kPmBp = 2244u` |  |
| `kPmLastSegs` | global | `plugin/engine/Engine.cpp:26` | `constexpr uint32_t kPmLastSegs = 2245u` |  |
| `kPmNy` | global | `plugin/engine/Engine.cpp:27` | `constexpr uint32_t kPmNy = 2249u` |  |
| `kPmMask` | global | `plugin/engine/Engine.cpp:28` | `constexpr uint32_t kPmMask = 2282u` |  |
| `kPmLock` | global | `plugin/engine/Engine.cpp:29` | `constexpr uint32_t kPmLock = 2411u` |  |
| `kPmActive` | global | `plugin/engine/Engine.cpp:30` | `constexpr uint32_t kPmActive = 2412u` |  |
| `current` | global | `plugin/engine/MidiBridge.cpp:8` | `thread_local p1::Engine* current = nullptr` |  |
| `P1_PHASE_READ` | macro | `plugin/engine/PhaseMapLayout.h:26` | `#define P1_PHASE_READ 0` |  |
| `kBaseWords` | global | `plugin/engine/PhaseMapLayout.h:31` | `constexpr uint32_t kBaseWords = 2560u` |  |
| `kTable` | global | `plugin/engine/PhaseMapLayout.h:32` | `constexpr uint32_t kTable = kBaseWords` | constexpr uint32_t kBaseWords = 2560u; /* the geometry, F, SEG and clocks |
| `kTableN` | global | `plugin/engine/PhaseMapLayout.h:35` | `constexpr uint32_t kTableN = 4096u` |  |
| `kTableStep` | global | `plugin/engine/PhaseMapLayout.h:36` | `constexpr uint32_t kTableStep = 1u` |  |
| `kTableExtra` | global | `plugin/engine/PhaseMapLayout.h:37` | `constexpr uint32_t kTableExtra = 0u` |  |
| `kTableN` | global | `plugin/engine/PhaseMapLayout.h:39` | `constexpr uint32_t kTableN = 512u` |  |
| `kTableStep` | global | `plugin/engine/PhaseMapLayout.h:40` | `constexpr uint32_t kTableStep = 8u` |  |
| `kTableExtra` | global | `plugin/engine/PhaseMapLayout.h:41` | `constexpr uint32_t kTableExtra = 0u` |  |
| `kTableN` | global | `plugin/engine/PhaseMapLayout.h:43` | `constexpr uint32_t kTableN = 512u` |  |
| `kTableStep` | global | `plugin/engine/PhaseMapLayout.h:44` | `constexpr uint32_t kTableStep = 8u` |  |
| `kTableExtra` | global | `plugin/engine/PhaseMapLayout.h:45` | `constexpr uint32_t kTableExtra = 1u` |  |
| `kTableN` | global | `plugin/engine/PhaseMapLayout.h:47` | `constexpr uint32_t kTableN = 1024u` |  |
| `kTableStep` | global | `plugin/engine/PhaseMapLayout.h:48` | `constexpr uint32_t kTableStep = 4u` |  |
| `kTableExtra` | global | `plugin/engine/PhaseMapLayout.h:49` | `constexpr uint32_t kTableExtra = 1u` |  |
| `kTableN` | global | `plugin/engine/PhaseMapLayout.h:51` | `constexpr uint32_t kTableN = 256u` |  |
| `kTableStep` | global | `plugin/engine/PhaseMapLayout.h:52` | `constexpr uint32_t kTableStep = 16u` |  |
| `kTableExtra` | global | `plugin/engine/PhaseMapLayout.h:53` | `constexpr uint32_t kTableExtra = 0u` |  |
| `kTableN` | global | `plugin/engine/PhaseMapLayout.h:55` | `constexpr uint32_t kTableN = 256u` |  |
| `kTableStep` | global | `plugin/engine/PhaseMapLayout.h:56` | `constexpr uint32_t kTableStep = 16u` |  |
| `kTableExtra` | global | `plugin/engine/PhaseMapLayout.h:57` | `constexpr uint32_t kTableExtra = 1u` |  |
| `kTableN` | global | `plugin/engine/PhaseMapLayout.h:59` | `constexpr uint32_t kTableN = 256u` |  |
| `kTableStep` | global | `plugin/engine/PhaseMapLayout.h:60` | `constexpr uint32_t kTableStep = 16u` |  |
| `kTableExtra` | global | `plugin/engine/PhaseMapLayout.h:61` | `constexpr uint32_t kTableExtra = 0u` |  |
| `kTableN` | global | `plugin/engine/PhaseMapLayout.h:63` | `constexpr uint32_t kTableN = 256u` |  |
| `kTableStep` | global | `plugin/engine/PhaseMapLayout.h:64` | `constexpr uint32_t kTableStep = 16u` |  |
| `kTableExtra` | global | `plugin/engine/PhaseMapLayout.h:65` | `constexpr uint32_t kTableExtra = 1u` |  |
| `kTableN` | global | `plugin/engine/PhaseMapLayout.h:67` | `constexpr uint32_t kTableN = 512u` |  |
| `kTableStep` | global | `plugin/engine/PhaseMapLayout.h:68` | `constexpr uint32_t kTableStep = 8u` |  |
| `kTableExtra` | global | `plugin/engine/PhaseMapLayout.h:69` | `constexpr uint32_t kTableExtra = 1u` |  |
| `kTableN` | global | `plugin/engine/PhaseMapLayout.h:71` | `constexpr uint32_t kTableN = 1024u` |  |
| `kTableStep` | global | `plugin/engine/PhaseMapLayout.h:72` | `constexpr uint32_t kTableStep = 4u` |  |
| `kTableExtra` | global | `plugin/engine/PhaseMapLayout.h:73` | `constexpr uint32_t kTableExtra = 1u` |  |
| `kTableN` | global | `plugin/engine/PhaseMapLayout.h:75` | `constexpr uint32_t kTableN = 1024u` |  |
| `kTableStep` | global | `plugin/engine/PhaseMapLayout.h:76` | `constexpr uint32_t kTableStep = 4u` |  |
| `kTableExtra` | global | `plugin/engine/PhaseMapLayout.h:77` | `constexpr uint32_t kTableExtra = 1u` |  |
| `kTableN` | global | `plugin/engine/PhaseMapLayout.h:79` | `constexpr uint32_t kTableN = 0u` |  |
| `kTableStep` | global | `plugin/engine/PhaseMapLayout.h:80` | `constexpr uint32_t kTableStep = 1u` |  |
| `kTableExtra` | global | `plugin/engine/PhaseMapLayout.h:81` | `constexpr uint32_t kTableExtra = 0u` |  |
| `kQDomain` | global | `plugin/engine/PhaseMapLayout.h:84` | `constexpr bool kQDomain = P1_PHASE_READ >= 7` |  |
| `kTableWords` | global | `plugin/engine/PhaseMapLayout.h:87` | `constexpr uint32_t kTableWords = (kTableN + kTableExtra + 1u) / 2u` | 16-bit entries, two per pool word, plus the interpolation tail. |
| `kWords` | global | `plugin/engine/PhaseMapLayout.h:88` | `constexpr uint32_t kWords = kTable + kTableWords` |  |
| `kPi` | global | `plugin/engine/RateBridge.cpp:12` | `constexpr double kPi = 3.14159265358979323846` |  |
| `CoreTapCallback` | alias | `plugin/engine/RateBridge.hpp:17` | `using CoreTapCallback = void (*)(void* context, Engine& engine, int frames)` |  |
| `kSpectrumGroupCount` | global | `plugin/juce/PagesSpectral.cpp:43` | `constexpr int kSpectrumGroupCount = 14` |  |
| `kSpectrumGroupNames` | global | `plugin/juce/PagesSpectral.cpp:44` | `const char* const kSpectrumGroupNames[kSpectrumGroupCount] = { "all", "odd", "even", "octave (P8)", "harmonic third (...` |  |
| `kSpectrumGroupLabels` | global | `plugin/juce/PagesSpectral.cpp:51` | `const char* const kSpectrumGroupLabels[kSpectrumGroupCount] = { "all", "odd", "even", "octave P8", "third P5", "fifth...` |  |
| `kSpectrumMaskNames` | global | `plugin/juce/PagesSpectral.cpp:55` | `const char* const kSpectrumMaskNames[3] = {"live", "dead", "both"}` |  |
| `kWidth` | global | `plugin/juce/PluginEditor.cpp:14` | `constexpr int kWidth = 1240` |  |
| `kHeight` | global | `plugin/juce/PluginEditor.cpp:15` | `constexpr int kHeight = 985` |  |
| `p1` | alias | `plugin/juce/PluginEditor.cpp:397` | `using namespace p1` |  |
| `ui` | alias | `plugin/juce/PluginEditor.cpp:398` | `using namespace p1::ui` |  |
| `kCoreRate` | global | `plugin/juce/PluginProcessor.cpp:20` | `constexpr double kCoreRate = 48000.0` |  |
| `kControlBlock` | global | `plugin/juce/PluginProcessor.cpp:21` | `constexpr int kControlBlock = 32` |  |
| `kStateVersion` | global | `plugin/juce/PluginProcessor.cpp:22` | `constexpr int kStateVersion = 15` |  |
| `kCutoffLawStateVersion` | global | `plugin/juce/PluginProcessor.cpp:25` | `constexpr int kCutoffLawStateVersion = 5` | The state version whose cutoff values already use the current law. A later version bump must not run the cutoff conversion again. |
| `kUiPageLayout` | global | `plugin/juce/PluginProcessor.cpp:27` | `constexpr int kUiPageLayout = 5` | Layout 5 inserts the macros page at 1, so every later page moves up one. |
| `kPhaseMapPageLayout` | global | `plugin/juce/PluginProcessor.cpp:29` | `constexpr int kPhaseMapPageLayout = 4` | Layout 4 inserted op phasemap at 4, which moved the three spectral pages. |
| `kIntToFloat` | global | `plugin/juce/PluginProcessor.cpp:30` | `constexpr float kIntToFloat = 1.0f / 32768.0f` |  |
| `kInsertionStateVersion` | global | `plugin/juce/PluginProcessor.cpp:34` | `constexpr int kInsertionStateVersion = 15` | The state version that inserted the half square and the two algorithms. A later version bump must not shift the stored values again. |
| `processBlock` | alias | `plugin/juce/PluginProcessor.h:57` | `using juce::AudioProcessor::processBlock` |  |
| `kVolumePositions` | global | `plugin/juce/VolumeLaw.h:14` | `inline constexpr int kVolumePositions = 1000` | The master volume fader. The host parameter is the fader position rather than a gain, so the law lives here and the processor and the header slider read the same numbers. Position 0 is silent, positions 1 to 750 rise logarithmically through 60 dB to unity at 750, and positions 751 to 1000 accelerate in dB to +12 dB. Above unity the curve is quadratic, so the gain rises ever faster. |
| `kVolumeUnity` | global | `plugin/juce/VolumeLaw.h:15` | `inline constexpr int kVolumeUnity = 750` |  |
| `kVolumeFloorDb` | global | `plugin/juce/VolumeLaw.h:16` | `inline constexpr double kVolumeFloorDb = -60.0` |  |
| `kVolumeCeilingDb` | global | `plugin/juce/VolumeLaw.h:17` | `inline constexpr double kVolumeCeilingDb = 12.0` |  |
| `kVolumeParamID` | global | `plugin/juce/VolumeLaw.h:21` | `inline constexpr const char* kVolumeParamID = "output_volume"` | The host parameter id. It sits outside the manifest because the volume trims the plugin output rather than a patch or a part. |
| `kCutoffPositionMax` | global | `plugin/preset/CutoffLaw.h:15` | `inline constexpr int32_t kCutoffPositionMax = 55296` |  |
| `kCutoffMinHz` | global | `plugin/preset/CutoffLaw.h:16` | `inline constexpr double kCutoffMinHz = 3.5` |  |
| `kCutoffOctaves` | global | `plugin/preset/CutoffLaw.h:17` | `inline constexpr double kCutoffOctaves = 12.0` |  |
| `kCutoffPi` | global | `plugin/preset/CutoffLaw.h:18` | `inline constexpr double kCutoffPi = 3.14159265358979323846` |  |
| `kPhaseParamsPerPart` | global | `plugin/preset/PresetStore.cpp:19` | `constexpr int kPhaseParamsPerPart = 116` | The version 16 phase block: 29 parameters for each of the four operators. |
| `kRingmodParamsPerPart` | global | `plugin/preset/PresetStore.cpp:23` | `constexpr int kRingmodParamsPerPart = 10` | The version 17 ringmod block: three pair on/off, three pair levels and four direct carrier gates. |
| `kKeyFollowParamsPerPart` | global | `plugin/preset/PresetStore.cpp:28` | `constexpr int kKeyFollowParamsPerPart = 3` | The version 18 key-follow block: the breakpoint and the key/velocity on/off switches are part parameters now. They are appended after every earlier part parameter, so old records only gain three default slots at the end. |
| `kOffsetEnvelopeBlocks` | global | `plugin/preset/PresetStore.cpp:32` | `constexpr int kOffsetEnvelopeBlocks = 5` | The five envelope blocks that gained a slot in version 13: one pitch and one feedback envelope for each operator. |
| `kFilterEnvelopeGrow` | global | `plugin/preset/PresetStore.cpp:39` | `constexpr int kFilterEnvelopeGrow = 2` | The five absolute envelope blocks that changed for version 14: the filter envelope and one spectral envelope for each operator. The spectral blocks grew from seven rates and levels to ten, netting two slots after the separate depth was removed. The filter block grew from eight to ten in the following step and kept its version byte. |
| `kSpecEnvelopeGrow` | global | `plugin/preset/PresetStore.cpp:40` | `constexpr int kSpecEnvelopeGrow = 2` |  |
| `kVersion13SpecDepthToBlock` | global | `plugin/preset/PresetStore.cpp:43` | `constexpr int kVersion13SpecDepthToBlock = 46` | In the version 13 layout the spectral depth sits 46 slots before the r1 of its rate block. Version 14 moved the depth into the block as `on`. |
| `kRate` | global | `plugin/tests/additive_bench.cpp:24` | `constexpr int kRate = 48000` |  |
| `kVoices` | global | `plugin/tests/additive_bench.cpp:25` | `constexpr int kVoices = 24` |  |
| `kOperators` | global | `plugin/tests/additive_bench.cpp:26` | `constexpr int kOperators = 4` |  |
| `kParts` | global | `plugin/tests/additive_bench.cpp:27` | `constexpr int kParts = 2` |  |
| `kTables` | global | `plugin/tests/additive_bench.cpp:28` | `constexpr int kTables = kOperators * kParts` |  |
| `kPi` | global | `plugin/tests/additive_bench.cpp:29` | `constexpr double kPi = 3.14159265358979323846` |  |
| `p1_additive_morph_start` | global | `plugin/tests/additive_engine_bench.cpp:19` | `int32_t p1_additive_morph_start` |  |
| `p1_additive_morph_step` | global | `plugin/tests/additive_engine_bench.cpp:20` | `int32_t p1_additive_morph_step` |  |
| `p1_additive_morph_now` | global | `plugin/tests/additive_engine_bench.cpp:21` | `int32_t p1_additive_morph_now` |  |
| `kRate` | global | `plugin/tests/additive_engine_bench.cpp:25` | `constexpr int kRate = 48000` |  |
| `kVoices` | global | `plugin/tests/additive_engine_bench.cpp:26` | `constexpr int kVoices = 24` |  |
| `Section` | alias | `plugin/tests/editor_snapshot.cpp:679` | `using Section = PluginEditor::OperatorSection` |  |
| `failures` | global | `plugin/tests/engine_parity.cpp:25` | `int failures = 0` |  |
| `kEngineNoteOn` | global | `plugin/tests/engine_render_reference.h:8` | `inline constexpr std::int32_t kEngineNoteOn[512] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0...` |  |
| `kEngineRelease` | global | `plugin/tests/engine_render_reference.h:54` | `inline constexpr std::int32_t kEngineRelease[512] = { -277, -277, -269, -269, -255, -255, -249, -249, -235, -235, -22...` |  |
| `kPi` | global | `plugin/tests/feedback_bipolar.cpp:17` | `constexpr double kPi = 3.14159265358979323846` |  |
| `kSampleRate` | global | `plugin/tests/feedback_bipolar.cpp:18` | `constexpr double kSampleRate = 48000.0` |  |
| `kFundamental` | global | `plugin/tests/feedback_bipolar.cpp:19` | `constexpr double kFundamental = 261.6255653005986` |  |
| `kSamples` | global | `plugin/tests/feedback_bipolar.cpp:20` | `constexpr int kSamples = 16384` | constexpr double kFundamental = 261.6255653005986; /* note 60 |
| `kWindowStart` | global | `plugin/tests/feedback_bipolar.cpp:21` | `constexpr int kWindowStart = 8192` |  |
| `kWindow` | global | `plugin/tests/feedback_bipolar.cpp:22` | `constexpr int kWindow = 8192` |  |
| `failures` | global | `plugin/tests/feedback_bipolar.cpp:24` | `int failures = 0` |  |
| `kPi` | global | `plugin/tests/feedback_envelope.cpp:18` | `constexpr double kPi = 3.14159265358979323846` |  |
| `kSampleRate` | global | `plugin/tests/feedback_envelope.cpp:19` | `constexpr double kSampleRate = 48000.0` |  |
| `kFundamental` | global | `plugin/tests/feedback_envelope.cpp:20` | `constexpr double kFundamental = 261.6255653005986` |  |
| `kSamples` | global | `plugin/tests/feedback_envelope.cpp:21` | `constexpr int kSamples = 16384` | constexpr double kFundamental = 261.6255653005986; /* note 60 |
| `kWindow` | global | `plugin/tests/feedback_envelope.cpp:22` | `constexpr int kWindow = 2048` |  |
| `failures` | global | `plugin/tests/feedback_envelope.cpp:24` | `int failures = 0` |  |
| `kBlock` | global | `plugin/tests/filter_envelope.cpp:15` | `constexpr int kBlock = 32` |  |
| `kTarget` | global | `plugin/tests/filter_envelope.cpp:16` | `constexpr int32_t kTarget = 99 * 256` |  |
| `failures` | global | `plugin/tests/filter_envelope.cpp:18` | `int failures = 0` |  |
| `failures` | global | `plugin/tests/filter_law.cpp:12` | `int failures = 0` |  |
| `kFull` | global | `plugin/tests/lfo_fades.cpp:18` | `constexpr uint32_t kFull = 0x80000000u` |  |
| `kVoices` | global | `plugin/tests/lfo_fades.cpp:19` | `constexpr int kVoices = 24` |  |
| `kTickFrames` | global | `plugin/tests/lfo_fades.cpp:20` | `constexpr int kTickFrames = 32` |  |
| `kMaxTicks` | global | `plugin/tests/lfo_fades.cpp:21` | `constexpr int kMaxTicks = 60000` |  |
| `kTicks1` | global | `plugin/tests/lfo_fades.cpp:23` | `constexpr int kTicks1 = 50298` | The envelope table's 99 percent times at 48 kHz, in control ticks. |
| `kTicks50` | global | `plugin/tests/lfo_fades.cpp:24` | `constexpr int kTicks50 = 3210` |  |
| `kTicks70` | global | `plugin/tests/lfo_fades.cpp:25` | `constexpr int kTicks70 = 727` |  |
| `kTicks90` | global | `plugin/tests/lfo_fades.cpp:26` | `constexpr int kTicks90 = 79` |  |
| `kTicks98` | global | `plugin/tests/lfo_fades.cpp:27` | `constexpr int kTicks98 = 12` |  |
| `failures` | global | `plugin/tests/lfo_fades.cpp:29` | `int failures = 0` |  |
| `kSamples` | global | `plugin/tests/operator_lfos.cpp:14` | `constexpr int kSamples = 16384` |  |
| `failures` | global | `plugin/tests/operator_lfos.cpp:15` | `int failures = 0` |  |
| `kVoices` | global | `plugin/tests/phase_map.cpp:22` | `constexpr int kVoices = 24` |  |
| `kTickFrames` | global | `plugin/tests/phase_map.cpp:23` | `constexpr int kTickFrames = 32` |  |
| `kWords` | global | `plugin/tests/phase_map.cpp:27` | `constexpr uint32_t kWords = p1::phase_map::kWords` | The phase_map pool layout, mirrored from the node. The stride follows the compiled read table. |
| `kNx` | global | `plugin/tests/phase_map.cpp:28` | `constexpr uint32_t kNx = 0u` |  |
| `kNslope` | global | `plugin/tests/phase_map.cpp:29` | `constexpr uint32_t kNslope = 33u` |  |
| `kNflip` | global | `plugin/tests/phase_map.cpp:30` | `constexpr uint32_t kNflip = 65u` |  |
| `kOrder` | global | `plugin/tests/phase_map.cpp:31` | `constexpr uint32_t kOrder = 97u` |  |
| `kNy` | global | `plugin/tests/phase_map.cpp:32` | `constexpr uint32_t kNy = 2249u` |  |
| `kMask` | global | `plugin/tests/phase_map.cpp:33` | `constexpr uint32_t kMask = 2282u` |  |
| `kLock` | global | `plugin/tests/phase_map.cpp:34` | `constexpr uint32_t kLock = 2411u` |  |
| `kActive` | global | `plugin/tests/phase_map.cpp:35` | `constexpr uint32_t kActive = 2412u` |  |
| `kBp` | global | `plugin/tests/phase_map.cpp:36` | `constexpr uint32_t kBp = 2244u` |  |
| `kLastSegs` | global | `plugin/tests/phase_map.cpp:37` | `constexpr uint32_t kLastSegs = 2245u` |  |
| `kDacc` | global | `plugin/tests/phase_map.cpp:38` | `constexpr uint32_t kDacc = 2246u` |  |
| `kFacc` | global | `plugin/tests/phase_map.cpp:39` | `constexpr uint32_t kFacc = 2247u` |  |
| `kRacc` | global | `plugin/tests/phase_map.cpp:40` | `constexpr uint32_t kRacc = 2248u` |  |
| `kF` | global | `plugin/tests/phase_map.cpp:41` | `constexpr uint32_t kF = 1217u` |  |
| `kPlRateLevel` | global | `plugin/tests/phase_map.cpp:44` | `constexpr uint32_t kPlRateLevel = 2u` | The phase_lfo and phase_env pools. |
| `kPlDepthLevel` | global | `plugin/tests/phase_map.cpp:45` | `constexpr uint32_t kPlDepthLevel = 3u` |  |
| `kPlValue` | global | `plugin/tests/phase_map.cpp:46` | `constexpr uint32_t kPlValue = 6u` |  |
| `kPeStage` | global | `plugin/tests/phase_map.cpp:47` | `constexpr uint32_t kPeStage = 0u` |  |
| `kPeLevel` | global | `plugin/tests/phase_map.cpp:48` | `constexpr uint32_t kPeLevel = 1u` |  |
| `failures` | global | `plugin/tests/phase_map.cpp:50` | `int failures = 0` |  |
| `kRate` | global | `plugin/tests/phase_map_perceptual.cpp:31` | `constexpr uint32_t kRate = 48000u` |  |
| `kTick` | global | `plugin/tests/phase_map_perceptual.cpp:32` | `constexpr int kTick = 32` |  |
| `kBase` | global | `plugin/tests/phase_map_perceptual.cpp:33` | `constexpr uint32_t kBase = p1::phase_map::kBaseWords` |  |
| `kNx` | global | `plugin/tests/phase_map_perceptual.cpp:36` | `constexpr uint32_t kNx = 0u` | The phase_map pool offsets, mirrored from the node. |
| `kNslope` | global | `plugin/tests/phase_map_perceptual.cpp:37` | `constexpr uint32_t kNslope = 33u` |  |
| `kNflip` | global | `plugin/tests/phase_map_perceptual.cpp:38` | `constexpr uint32_t kNflip = 65u` |  |
| `kOrder` | global | `plugin/tests/phase_map_perceptual.cpp:39` | `constexpr uint32_t kOrder = 97u` |  |
| `kPy0` | global | `plugin/tests/phase_map_perceptual.cpp:40` | `constexpr uint32_t kPy0 = 129u` |  |
| `kPslope` | global | `plugin/tests/phase_map_perceptual.cpp:41` | `constexpr uint32_t kPslope = 161u` |  |
| `kSeg` | global | `plugin/tests/phase_map_perceptual.cpp:42` | `constexpr uint32_t kSeg = 193u` |  |
| `kF` | global | `plugin/tests/phase_map_perceptual.cpp:43` | `constexpr uint32_t kF = 1217u` |  |
| `kBp` | global | `plugin/tests/phase_map_perceptual.cpp:44` | `constexpr uint32_t kBp = 2244u` |  |
| `kLastSegs` | global | `plugin/tests/phase_map_perceptual.cpp:45` | `constexpr uint32_t kLastSegs = 2245u` |  |
| `kDacc` | global | `plugin/tests/phase_map_perceptual.cpp:46` | `constexpr uint32_t kDacc = 2246u` |  |
| `kFacc` | global | `plugin/tests/phase_map_perceptual.cpp:47` | `constexpr uint32_t kFacc = 2247u` |  |
| `kRacc` | global | `plugin/tests/phase_map_perceptual.cpp:48` | `constexpr uint32_t kRacc = 2248u` |  |
| `kSeed` | global | `plugin/tests/phase_map_perceptual.cpp:49` | `constexpr uint32_t kSeed = 2242u` |  |
| `kSeeded` | global | `plugin/tests/phase_map_perceptual.cpp:50` | `constexpr uint32_t kSeeded = 2243u` |  |
| `kActive` | global | `plugin/tests/phase_map_perceptual.cpp:51` | `constexpr uint32_t kActive = 2412u` |  |
| `kLastB` | global | `plugin/tests/phase_map_perceptual.cpp:52` | `constexpr uint32_t kLastB = 2241u` |  |
| `kPi` | global | `plugin/tests/phase_map_perceptual_compare.cpp:29` | `constexpr double kPi = 3.14159265358979323846` |  |
| `kN` | global | `plugin/tests/phase_map_perceptual_compare.cpp:30` | `constexpr int kN = 4096` |  |
| `kHop` | global | `plugin/tests/phase_map_perceptual_compare.cpp:31` | `constexpr int kHop = 1024` | constexpr int kN = 4096;         /* spectral frame |
| `kEnvBlock` | global | `plugin/tests/phase_map_perceptual_compare.cpp:32` | `constexpr int kEnvBlock = 240` |  |
| `kLsdMax` | global | `plugin/tests/phase_map_perceptual_compare.cpp:36` | `constexpr double kLsdMax = 0.50` | The perceptual gate. The spectral and envelope limits are the contract limits. The null depth is a sanity floor, not a substitute for them. |
| `kLsdP95` | global | `plugin/tests/phase_map_perceptual_compare.cpp:37` | `constexpr double kLsdP95 = 0.25` |  |
| `kBandMax` | global | `plugin/tests/phase_map_perceptual_compare.cpp:38` | `constexpr double kBandMax = 0.50` |  |
| `kCentroidPct` | global | `plugin/tests/phase_map_perceptual_compare.cpp:39` | `constexpr double kCentroidPct = 2.0` |  |
| `kRolloffPct` | global | `plugin/tests/phase_map_perceptual_compare.cpp:40` | `constexpr double kRolloffPct = 2.0` |  |
| `kFlatness` | global | `plugin/tests/phase_map_perceptual_compare.cpp:41` | `constexpr double kFlatness = 0.02` |  |
| `kEnvMax` | global | `plugin/tests/phase_map_perceptual_compare.cpp:42` | `constexpr double kEnvMax = 0.50` |  |
| `kEnvP95` | global | `plugin/tests/phase_map_perceptual_compare.cpp:43` | `constexpr double kEnvP95 = 0.25` |  |
| `kHarmMax` | global | `plugin/tests/phase_map_perceptual_compare.cpp:44` | `constexpr double kHarmMax = 0.50` |  |
| `kAliasDelta` | global | `plugin/tests/phase_map_perceptual_compare.cpp:45` | `constexpr double kAliasDelta = 0.50` |  |
| `kAttackMs` | global | `plugin/tests/phase_map_perceptual_compare.cpp:46` | `constexpr double kAttackMs = 1.0` |  |
| `kReleaseMs` | global | `plugin/tests/phase_map_perceptual_compare.cpp:47` | `constexpr double kReleaseMs = 1.0` |  |
| `kNullDb` | global | `plugin/tests/phase_map_perceptual_compare.cpp:48` | `constexpr double kNullDb = -40.0` |  |
| `kRmsDeltaDb` | global | `plugin/tests/phase_map_perceptual_compare.cpp:49` | `constexpr double kRmsDeltaDb = 0.10` |  |
| `kPeakDeltaDb` | global | `plugin/tests/phase_map_perceptual_compare.cpp:50` | `constexpr double kPeakDeltaDb = 0.10` |  |
| `kBlock` | global | `plugin/tests/pitch_envelope.cpp:15` | `constexpr int kBlock = 32` |  |
| `kBlocks` | global | `plugin/tests/pitch_envelope.cpp:16` | `constexpr int kBlocks = 512` |  |
| `failures` | global | `plugin/tests/pitch_envelope.cpp:18` | `int failures = 0` |  |
| `kFrames` | global | `plugin/tests/plugin_smoke.cpp:28` | `constexpr int kFrames = 256` |  |
| `failures` | global | `plugin/tests/plugin_smoke.cpp:30` | `int failures = 0` |  |
| `kFrames` | global | `plugin/tests/plugin_stability.cpp:17` | `constexpr int kFrames = 256` |  |
| `failures` | global | `plugin/tests/plugin_stability.cpp:18` | `int failures = 0` |  |
| `failures` | global | `plugin/tests/preset_smoke.cpp:23` | `int failures = 0` |  |
| `kCoreRate` | global | `plugin/tests/rate_bridge_test.cpp:14` | `constexpr double kCoreRate = 48000.0` |  |
| `kPi` | global | `plugin/tests/rate_bridge_test.cpp:15` | `constexpr double kPi = 3.14159265358979323846` |  |
| `kAmplitude` | global | `plugin/tests/rate_bridge_test.cpp:16` | `constexpr double kAmplitude = 16000.0` |  |
| `kRate` | global | `plugin/tests/spectral.cpp:18` | `constexpr double kRate = 48000.0` |  |
| `kPi` | global | `plugin/tests/spectral.cpp:19` | `constexpr double kPi = 3.14159265358979323846` |  |
| `kFrames` | global | `plugin/tests/spectral.cpp:20` | `constexpr int kFrames = 44100` |  |
| `kWindow` | global | `plugin/tests/spectral.cpp:21` | `constexpr int kWindow = 4096` |  |
| `kF0` | global | `plugin/tests/spectral.cpp:22` | `constexpr double kF0 = 261.6255653005986` |  |
| `kRate` | global | `plugin/tests/spectral_bench.cpp:21` | `constexpr int kRate = 48000` |  |
| `kControlBlock` | global | `plugin/tests/spectral_bench.cpp:22` | `constexpr int kControlBlock = 32` |  |
| `kWidth` | global | `plugin/tests/ui_frame_bench.cpp:20` | `constexpr int kWidth = 330` |  |
| `kHeight` | global | `plugin/tests/ui_frame_bench.cpp:21` | `constexpr int kHeight = 50` |  |
| `kFrames` | global | `plugin/tests/ui_frame_bench.cpp:22` | `constexpr int kFrames = 2000` |  |
| `kVoices` | global | `plugin/tests/voice_alloc.cpp:12` | `constexpr int kVoices = 24` |  |
| `failures` | global | `plugin/tests/voice_alloc.cpp:14` | `int failures = 0` |  |
| `kFnvOffset` | global | `plugin/tests/wavetable.cpp:14` | `constexpr uint64_t kFnvOffset = 0xcbf29ce484222325ull` |  |
| `kFnvPrime` | global | `plugin/tests/wavetable.cpp:15` | `constexpr uint64_t kFnvPrime = 0x100000001b3ull` |  |
| `_HERE` | module constant | `plugin/tools/additive_prototype.py:19` | `_HERE = Path(__file__).resolve().parent` |  |
| `_LEVELS` | module constant | `plugin/tools/additive_prototype.py:20` | `_LEVELS = 9` |  |
| `_SIZE` | module constant | `plugin/tools/additive_prototype.py:21` | `_SIZE = 1024` |  |
| `_HARMONICS` | module constant | `plugin/tools/additive_prototype.py:22` | `_HARMONICS = 16` |  |
| `raw` | module constant | `plugin/tools/additive_prototype.py:26` | `raw = []` |  |
| `total` | module constant | `plugin/tools/additive_prototype.py:27` | `total = 0.0` |  |
| `value` | module constant | `plugin/tools/additive_prototype.py:29` | `value = h ** (-tilt)` |  |
| `distance` | module constant | `plugin/tools/additive_prototype.py:31` | `distance = h - formant` |  |
| `scale` | module constant | `plugin/tools/additive_prototype.py:35` | `scale = 0.95 / total if total > 0.0 else 0.0` |  |
| `limit` | module constant | `plugin/tools/additive_prototype.py:40` | `limit = max(1, _HARMONICS >> level)` |  |
| `values` | module constant | `plugin/tools/additive_prototype.py:41` | `values = []` |  |
| `total` | module constant | `plugin/tools/additive_prototype.py:43` | `total = 0` |  |
| `lines` | module constant | `plugin/tools/additive_prototype.py:52` | `lines = [f"static const int16_t {name}[{_LEVELS * _SIZE}] = {{"]` |  |
| `chunk` | module constant | `plugin/tools/additive_prototype.py:54` | `chunk = ", ".join(str(value) for value in table[start:start + 16])` |  |
| `dark` | module constant | `plugin/tools/additive_prototype.py:61` | `dark = _recipe(_HARMONICS, 2.2, 0, 0.0)` |  |
| `bright` | module constant | `plugin/tools/additive_prototype.py:62` | `bright = _recipe(_HARMONICS, 0.7, 6, 1.2)` |  |
| `dark_tables` | module constant | `plugin/tools/additive_prototype.py:63` | `dark_tables = []` |  |
| `bright_tables` | module constant | `plugin/tools/additive_prototype.py:64` | `bright_tables = []` |  |
| `pattern` | module constant | `plugin/tools/additive_prototype.py:114` | `pattern = re.compile( r"(\n[ \t]*)op([1-4])_osc_audio_x = p1_wave\[op\2_osc_base\[v\]\s*\n" r"\s*\+ \(\(op\2_osc_bl\[...` |  |
| `indent` | module constant | `plugin/tools/additive_prototype.py:121` | `indent = match.group(1)` |  |
| `op` | module constant | `plugin/tools/additive_prototype.py:122` | `op = match.group(2)` |  |
| `patched` | module constant | `plugin/tools/additive_prototype.py:134` | `patched = patched.replace( '#include "tables.h"', '#include "tables.h"\n#include "additive_tables.h"\n\n' "int32_t p1...` |  |
| `loop` | module constant | `plugin/tools/additive_prototype.py:144` | `loop = "for (uint32_t f = b; f < b + 32u && f < frames; f++) {"` |  |
| `patched` | module constant | `plugin/tools/additive_prototype.py:147` | `patched = patched.replace( loop, loop + "\n p1_additive_morph_now = p1_additive_morph_start" " + p1_additive_morph_st...` |  |
| `parser` | module constant | `plugin/tools/additive_prototype.py:158` | `parser = argparse.ArgumentParser(prog="additive_prototype")` |  |
| `args` | module constant | `plugin/tools/additive_prototype.py:163` | `args = parser.parse_args()` |  |
| `source` | module constant | `plugin/tools/additive_prototype.py:164` | `source = (args.engine / "type_p_alpha_dsp.c").read_text(encoding="utf-8")` |  |
| `_SAMPLE_RATE` | module constant | `plugin/tools/feedback_figures.py:22` | `_SAMPLE_RATE = 44100` |  |
| `_PANEL_WIDTH` | module constant | `plugin/tools/feedback_figures.py:23` | `_PANEL_WIDTH = 900` |  |
| `_PANEL_HEIGHT` | module constant | `plugin/tools/feedback_figures.py:24` | `_PANEL_HEIGHT = 160` |  |
| `_WINDOW` | module constant | `plugin/tools/feedback_figures.py:25` | `_WINDOW = 2048` |  |
| `_BG` | module constant | `plugin/tools/feedback_figures.py:26` | `_BG = (0, 0, 0)` |  |
| `_GRID` | module constant | `plugin/tools/feedback_figures.py:27` | `_GRID = (0x1a, 0x1a, 0x1a)` |  |
| `_ZERO` | module constant | `plugin/tools/feedback_figures.py:28` | `_ZERO = (0x3a, 0x3a, 0x3a)` |  |
| `_LINE` | module constant | `plugin/tools/feedback_figures.py:29` | `_LINE = (0x3d, 0xff, 0x9e)` |  |
| `_GROUPS` | module constant | `plugin/tools/feedback_figures.py:31` | `_GROUPS = ( ("positive", "positive"), ("square-avg", "square-avg"), ("square-lp", "square-lp"), ("abs-avg", "abs-avg"...` |  |
| `_LEVELS` | module constant | `plugin/tools/feedback_figures.py:38` | `_LEVELS = ("25", "50", "75", "100")` |  |
| `frames` | module constant | `plugin/tools/feedback_figures.py:45` | `frames = handle.getnframes()` |  |
| `data` | module constant | `plugin/tools/feedback_figures.py:46` | `data = handle.readframes(frames)` |  |
| `raw` | module constant | `plugin/tools/feedback_figures.py:60` | `raw = b"".join(b"\x00" + bytes(row) for row in rows)` |  |
| `header` | module constant | `plugin/tools/feedback_figures.py:61` | `header = struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0)` |  |
| `data` | module constant | `plugin/tools/feedback_figures.py:62` | `data = ( b"\x89PNG\r\n\x1a\n" + _chunk(b"IHDR", header) + _chunk(b"IDAT", zlib.compress(raw, 9)) + _chunk(b"IEND", b"...` |  |
| `offset` | module constant | `plugin/tools/feedback_figures.py:73` | `offset = x * 3` |  |
| `rows` | module constant | `plugin/tools/feedback_figures.py:80` | `rows = [bytearray(_BG * _PANEL_WIDTH) for _ in range(_PANEL_HEIGHT)]` |  |
| `centre` | module constant | `plugin/tools/feedback_figures.py:87` | `centre = _PANEL_HEIGHT // 2` |  |
| `window` | module constant | `plugin/tools/feedback_figures.py:91` | `window = samples[-_WINDOW:] if len(samples) > _WINDOW else samples` |  |
| `previous` | module constant | `plugin/tools/feedback_figures.py:92` | `previous = centre` |  |
| `index` | module constant | `plugin/tools/feedback_figures.py:94` | `index = int(x * (len(window) - 1) / max(1, _PANEL_WIDTH - 1))` |  |
| `value` | module constant | `plugin/tools/feedback_figures.py:95` | `value = window[index] / 32768.0` |  |
| `y` | module constant | `plugin/tools/feedback_figures.py:96` | `y = int(centre - value * (centre - 8))` |  |
| `y` | module constant | `plugin/tools/feedback_figures.py:97` | `y = max(0, min(_PANEL_HEIGHT - 1, y))` |  |
| `previous` | module constant | `plugin/tools/feedback_figures.py:101` | `previous = y` |  |
| `parser` | module constant | `plugin/tools/feedback_figures.py:106` | `parser = argparse.ArgumentParser(description=__doc__)` |  |
| `args` | module constant | `plugin/tools/feedback_figures.py:108` | `args = parser.parse_args()` |  |
| `rows` | module constant | `plugin/tools/feedback_figures.py:111` | `rows: list[bytearray] = []` |  |
| `path` | module constant | `plugin/tools/feedback_figures.py:113` | `path = args.directory / f"{prefix}-{level}.wav"` |  |
| `out` | module constant | `plugin/tools/feedback_figures.py:117` | `out = args.directory / f"scope-{group}.png"` |  |
| `amp` | module constant | `plugin/tools/gen_macro_targets.py:28` | `amp = [ "op1.eg.warm.p{part}", "attack.p{part}", "op1.eg.d1r.p{part}", "op1.eg.d2r.p{part}", "release.p{part}", ]` |  |
| `amp` | module constant | `plugin/tools/gen_macro_targets.py:36` | `amp = [ f"op{op}.eg.warm.p{{part}}", f"op{op}.eg.surge.p{{part}}", f"op{op}.eg.d1r.p{{part}}", f"op{op}.eg.d2r.p{{par...` |  |
| `FAMILIES` | module constant | `plugin/tools/gen_macro_targets.py:59` | `FAMILIES: dict[str, list[str]] = { "op_env_rates": [], # built per operator by op_env_rates() "op_lfo_rates": [ "op{o...` |  |
| `VELOCITY_DEPTH` | module constant | `plugin/tools/gen_macro_targets.py:81` | `VELOCITY_DEPTH = [ *[f"op{op}_{field}_vs.p{{part}}" for op in range(1, 5) for field in ("level", "fb", "aenv")], "pen...` |  |
| `p1_start` | module constant | `plugin/tools/gen_macro_targets.py:108` | `p1_start = next( (i for i, p in enumerate(params) if str(p["name"]).endswith(".p1")), None, )` |  |
| `p1_end` | module constant | `plugin/tools/gen_macro_targets.py:114` | `p1_end = next( ( i for i in range(p1_start + 1, len(params)) if not str(params[i]["name"]).endswith(".p1") ), len(par...` |  |
| `ring` | module constant | `plugin/tools/gen_macro_targets.py:124` | `ring = [p for p in group if is_ring_param(str(p["name"]))]` |  |
| `text` | module constant | `plugin/tools/gen_macro_targets.py:136` | `text = tables_path.read_text(encoding="utf-8")` |  |
| `match` | module constant | `plugin/tools/gen_macro_targets.py:137` | `match = re.search(r"table_routing\[(\d+)\]\s*=\s*\{(.*?)\};", text, re.S)` |  |
| `declared` | module constant | `plugin/tools/gen_macro_targets.py:140` | `declared = int(match.group(1))` |  |
| `values` | module constant | `plugin/tools/gen_macro_targets.py:141` | `values = [int(v) for v in re.findall(r"-?\d+", match.group(2))]` |  |
| `family` | module constant | `plugin/tools/gen_macro_targets.py:158` | `family = entry["family"]` |  |
| `names` | module constant | `plugin/tools/gen_macro_targets.py:161` | `names: list[tuple[str, int]] = []` |  |
| `templates` | module constant | `plugin/tools/gen_macro_targets.py:163` | `templates = op_env_rates(op) if family == "op_env_rates" else FAMILIES[family]` |  |
| `spec` | module constant | `plugin/tools/gen_macro_targets.py:177` | `spec = cast(dict[str, Any], json.loads(Path(sys.argv[1]).read_text(encoding="utf-8")))` |  |
| `manifest` | module constant | `plugin/tools/gen_macro_targets.py:178` | `manifest = cast( dict[str, Any], json.loads(Path(sys.argv[2]).read_text(encoding="utf-8")) )` |  |
| `carriers` | module constant | `plugin/tools/gen_macro_targets.py:181` | `carriers = load_carriers(Path(sys.argv[3]))` |  |
| `params` | module constant | `plugin/tools/gen_macro_targets.py:182` | `params = reorder_ring(cast(list[dict[str, Any]], manifest["params"]))` |  |
| `known` | module constant | `plugin/tools/gen_macro_targets.py:183` | `known = {str(param["name"]): i for i, param in enumerate(params)}` |  |
| `curves` | module constant | `plugin/tools/gen_macro_targets.py:184` | `curves = {str(param["name"]): str(param["curve"]) for param in params}` |  |
| `target_max` | module constant | `plugin/tools/gen_macro_targets.py:185` | `target_max = next( (int(param["max"]) for param in params if param["name"] == "macro1_target1"), -1, )` |  |
| `sections` | module constant | `plugin/tools/gen_macro_targets.py:189` | `sections = [str(section) for section in spec["sections"]]` |  |
| `entries` | module constant | `plugin/tools/gen_macro_targets.py:190` | `entries = cast(list[dict[str, Any]], spec["targets"])` |  |
| `role_ids` | module constant | `plugin/tools/gen_macro_targets.py:191` | `role_ids = {"all": 0, "carrier": 1, "modulator": 2}` |  |
| `rows` | module constant | `plugin/tools/gen_macro_targets.py:193` | `rows: list[tuple[str, str]] = []` |  |
| `pool` | module constant | `plugin/tools/gen_macro_targets.py:194` | `pool: list[tuple[int, int, bool]] = []` |  |
| `offsets` | module constant | `plugin/tools/gen_macro_targets.py:195` | `offsets = [0]` |  |
| `roles` | module constant | `plugin/tools/gen_macro_targets.py:196` | `roles: list[int] = []` |  |
| `parts` | module constant | `plugin/tools/gen_macro_targets.py:197` | `parts: list[int] = []` |  |
| `logs` | module constant | `plugin/tools/gen_macro_targets.py:198` | `logs: list[bool] = []` |  |
| `target_index` | module constant | `plugin/tools/gen_macro_targets.py:199` | `target_index: dict[tuple[str, int], int] = {}` |  |
| `names` | module constant | `plugin/tools/gen_macro_targets.py:202` | `names = union_members(entry, suffix)` |  |
| `unknown` | module constant | `plugin/tools/gen_macro_targets.py:203` | `unknown = [name for name, _ in names if name not in known]` |  |
| `engine_rate_ids` | module constant | `plugin/tools/gen_macro_targets.py:228` | `engine_rate_ids = sorted( { known[name] for entry in entries if entry["name"] == "op env rates" for _, suffix in ((1,...` |  |
| `lfo_rate_ids` | module constant | `plugin/tools/gen_macro_targets.py:245` | `lfo_rate_ids = sorted( { known[name] for entry in entries if entry["name"] == "op lfo rates" for _, suffix in ((1, "0...` |  |
| `lfo_depth_ids` | module constant | `plugin/tools/gen_macro_targets.py:259` | `lfo_depth_ids = sorted( { known[name] for entry in entries if entry["name"] == "op lfo depths" for _, suffix in ((1, ...` |  |
| `lines` | module constant | `plugin/tools/gen_macro_targets.py:271` | `lines = [ "/* GENERATED by plugin/tools/gen_macro_targets.py - do not edit. */", "#pragma once", "#include <algorithm...` |  |
| `target` | module constant | `plugin/tools/gen_macro_targets.py:375` | `target = Path(sys.argv[4])` |  |
| `c_lines` | module constant | `plugin/tools/gen_macro_targets.py:378` | `c_lines = [ "/* GENERATED by plugin/tools/gen_macro_targets.py - do not edit. */", "#pragma once", f"#define P1_MACRO...` |  |
| `ids` | module constant | `plugin/tools/gen_macro_targets.py:423` | `ids = Path(sys.argv[5])` |  |
| `part` | module constant | `plugin/tools/gen_params.py:44` | `part = ""` |  |
| `part` | module constant | `plugin/tools/gen_params.py:46` | `part = f" p{int(name[-1]) + 1}"` |  |
| `name` | module constant | `plugin/tools/gen_params.py:47` | `name = name[:-3]` |  |
| `name` | module constant | `plugin/tools/gen_params.py:48` | `name = name.replace("_", " ").replace(".", " ")` |  |
| `p1_start` | module constant | `plugin/tools/gen_params.py:66` | `p1_start = next( (i for i, p in enumerate(params) if str(p["name"]).endswith(".p1")), None, )` |  |
| `p1_end` | module constant | `plugin/tools/gen_params.py:72` | `p1_end = next( ( i for i in range(p1_start + 1, len(params)) if not str(params[i]["name"]).endswith(".p1") ), len(par...` |  |
| `ring` | module constant | `plugin/tools/gen_params.py:82` | `ring = [p for p in group if is_ring_param(str(p["name"]))]` |  |
| `key_names` | module constant | `plugin/tools/gen_params.py:99` | `key_names = {"breakpoint", "key_sens_on", "vel_sens_on"}` |  |
| `p1_start` | module constant | `plugin/tools/gen_params.py:104` | `p1_start = next( (i for i, p in enumerate(params) if str(p["name"]).endswith(".p1")), None, )` |  |
| `p1_end` | module constant | `plugin/tools/gen_params.py:110` | `p1_end = next( ( i for i in range(p1_start + 1, len(params)) if not str(params[i]["name"]).endswith(".p1") ), len(par...` |  |
| `key` | module constant | `plugin/tools/gen_params.py:120` | `key = [p for p in group if is_key(str(p["name"]))]` |  |
| `source` | module constant | `plugin/tools/gen_params.py:138` | `source = Path(sys.argv[1])` |  |
| `target` | module constant | `plugin/tools/gen_params.py:139` | `target = Path(sys.argv[2])` |  |
| `raw` | module constant | `plugin/tools/gen_params.py:140` | `raw = cast(object, json.loads(source.read_text(encoding="utf-8")))` |  |
| `data` | module constant | `plugin/tools/gen_params.py:141` | `data = cast(ParamsDoc, raw)` |  |
| `params` | module constant | `plugin/tools/gen_params.py:142` | `params = reorder_key(reorder_ring(data["params"]))` |  |
| `ratio_params` | module constant | `plugin/tools/gen_params.py:145` | `ratio_params = [p for p in params if p["display"] == "ratio"]` |  |
| `ratio_detents_values` | module constant | `plugin/tools/gen_params.py:146` | `ratio_detents_values: list[int] = []` |  |
| `ratio_detents_values` | module constant | `plugin/tools/gen_params.py:148` | `ratio_detents_values = ratio_detents( min(p["min"] for p in ratio_params), max(p["max"] for p in ratio_params), )` |  |
| `names` | module constant | `plugin/tools/gen_params.py:152` | `names = [str(p["name"]) for p in params]` |  |
| `routing_prefixes` | module constant | `plugin/tools/gen_params.py:153` | `routing_prefixes = ("channel.", "low.", "high.", "level.", "delay.")` |  |
| `part_param_count` | module constant | `plugin/tools/gen_params.py:154` | `part_param_count = sum( name.endswith(".p0") and not name.startswith(routing_prefixes) for name in names )` |  |
| `part_count` | module constant | `plugin/tools/gen_params.py:157` | `part_count = len({name[-2:] for name in names if name.endswith((".p0", ".p1"))})` |  |
| `routing_count` | module constant | `plugin/tools/gen_params.py:158` | `routing_count = sum(name.startswith(routing_prefixes) for name in names)` |  |
| `routing_base` | module constant | `plugin/tools/gen_params.py:159` | `routing_base = int(data.get("routing_base", len(params) - routing_count))` |  |
| `base_count` | module constant | `plugin/tools/gen_params.py:160` | `base_count = int(data.get("base_param_count", len(params)))` |  |
| `part_cutoff_index` | module constant | `plugin/tools/gen_params.py:161` | `part_cutoff_index = next( (i for i, p in enumerate(params) if p["name"] == "cutoff.p0"), -1 )` |  |
| `neighbour` | module constant | `plugin/tools/gen_params.py:165` | `neighbour = params[part_param_count + part_cutoff_index]["name"]` |  |
| `lines` | module constant | `plugin/tools/gen_params.py:171` | `lines = [ "/* GENERATED by plugin/tools/gen_params.py - do not edit. */", "#pragma once", "#include <array>", "#inclu...` |  |
| `row` | module constant | `plugin/tools/gen_params.py:195` | `row = ( f"{p['id']}, " + c_string(p["name"]) + ", " + c_string(display_name(p["name"])) + ", " + c_string(p["unit"]) ...` |  |
| `macro_ids` | module constant | `plugin/tools/gen_params.py:226` | `macro_ids = [p["name"] for p in params if str(p["name"]).startswith("macro")]` |  |
| `value` | module constant | `plugin/tools/gen_tokens.py:25` | `value = hex_colour.lstrip("#")` |  |
| `source` | module constant | `plugin/tools/gen_tokens.py:35` | `source = Path(sys.argv[1])` |  |
| `target` | module constant | `plugin/tools/gen_tokens.py:36` | `target = Path(sys.argv[2])` |  |
| `raw` | module constant | `plugin/tools/gen_tokens.py:37` | `raw = cast(object, json.loads(source.read_text(encoding="utf-8")))` |  |
| `data` | module constant | `plugin/tools/gen_tokens.py:38` | `data = cast(_TokenFile, raw)` |  |
| `lines` | module constant | `plugin/tools/gen_tokens.py:40` | `lines = [ "/* GENERATED by plugin/tools/gen_tokens.py - do not edit. */", "#pragma once", "#include <cstdint>", "", "...` |  |
| `token` | module constant | `plugin/tools/gen_tokens.py:57` | `token = size_name[0].upper() + size_name[1:]` |  |
| `control_dict` | module constant | `plugin/tools/gen_tokens.py:72` | `control_dict = cast(dict[str, int], control_value)` |  |
| `token` | module constant | `plugin/tools/gen_tokens.py:83` | `token = font_name[0].upper() + font_name[1:]` |  |
| `_Q16` | module constant | `plugin/tools/ratio.py:9` | `_Q16 = 65536` |  |
| `_RATIO_BASES` | module constant | `plugin/tools/ratio.py:14` | `_RATIO_BASES: tuple[float, ...] = ( 0.5, 1.0, math.sqrt(2.0), math.pi / 2.0, math.sqrt(3.0), math.sqrt(2.0) / 2.0, ma...` |  |
| `ratio` | module constant | `plugin/tools/ratio.py:28` | `ratio = value / _Q16` |  |
| `text` | module constant | `plugin/tools/ratio.py:29` | `text = f"{ratio:.2f}"` |  |
| `groups` | module constant | `plugin/tools/ratio.py:37` | `groups: dict[str, list[int]] = {}` |  |
| `k` | module constant | `plugin/tools/ratio.py:39` | `k = 1` |  |
| `value` | module constant | `plugin/tools/ratio.py:41` | `value = math.ceil(base * k * _Q16)` |  |
| `kept` | module constant | `plugin/tools/ratio.py:47` | `kept: list[int] = []` |  |
| `_MANIFEST` | module constant | `plugin/tools/snapshot_check.py:26` | `_MANIFEST = "manifest.json"` |  |
| `digest` | module constant | `plugin/tools/snapshot_check.py:30` | `digest = hashlib.sha256()` |  |
| `header` | module constant | `plugin/tools/snapshot_check.py:40` | `header = handle.read(24)` |  |
| `path` | module constant | `plugin/tools/snapshot_check.py:54` | `path = golden / _MANIFEST` |  |
| `raw` | module constant | `plugin/tools/snapshot_check.py:57` | `raw = json.loads(path.read_text(encoding="utf-8"))` |  |
| `images` | module constant | `plugin/tools/snapshot_check.py:58` | `images = raw.get("images", {})` |  |
| `entries` | module constant | `plugin/tools/snapshot_check.py:65` | `entries: dict[str, dict[str, object]] = {}` |  |
| `payload` | module constant | `plugin/tools/snapshot_check.py:73` | `payload = {"platform": golden.name, "images": entries}` |  |
| `written` | module constant | `plugin/tools/snapshot_check.py:86` | `written: list[str] = []` |  |
| `current_path` | module constant | `plugin/tools/snapshot_check.py:88` | `current_path = actual / name` |  |
| `golden_path` | module constant | `plugin/tools/snapshot_check.py:89` | `golden_path = golden / name` |  |
| `old` | module constant | `plugin/tools/snapshot_check.py:96` | `old = old_image.convert("RGB")` |  |
| `new` | module constant | `plugin/tools/snapshot_check.py:97` | `new = new_image.convert("RGB")` |  |
| `new` | module constant | `plugin/tools/snapshot_check.py:99` | `new = new.resize(old.size)` |  |
| `difference` | module constant | `plugin/tools/snapshot_check.py:100` | `difference = ImageChops.difference(old, new)` |  |
| `bbox` | module constant | `plugin/tools/snapshot_check.py:101` | `bbox = difference.getbbox()` |  |
| `grey` | module constant | `plugin/tools/snapshot_check.py:104` | `grey = difference.convert("L")` |  |
| `overlay` | module constant | `plugin/tools/snapshot_check.py:105` | `overlay = Image.new("RGB", old.size, (0, 0, 0))` |  |
| `blended` | module constant | `plugin/tools/snapshot_check.py:107` | `blended = Image.blend(old, overlay, 0.55)` |  |
| `side` | module constant | `plugin/tools/snapshot_check.py:109` | `side = Image.new("RGB", (old.width * 2 + 8, old.height), (24, 24, 24))` |  |
| `parser` | module constant | `plugin/tools/snapshot_check.py:118` | `parser = argparse.ArgumentParser(prog="snapshot_check", description=__doc__)` |  |
| `args` | module constant | `plugin/tools/snapshot_check.py:127` | `args = parser.parse_args(argv)` |  |
| `actual` | module constant | `plugin/tools/snapshot_check.py:129` | `actual = _images(args.actual)` |  |
| `golden` | module constant | `plugin/tools/snapshot_check.py:149` | `golden = _images(args.golden)` |  |
| `manifest` | module constant | `plugin/tools/snapshot_check.py:150` | `manifest = _load_manifest(args.golden)` |  |
| `changed` | module constant | `plugin/tools/snapshot_check.py:160` | `changed: list[str] = []` |  |
| `problems` | module constant | `plugin/tools/snapshot_check.py:161` | `problems: list[str] = []` |  |
| `written` | module constant | `plugin/tools/snapshot_check.py:183` | `written = _diff_images(args.actual, args.golden, args.diff, changed)` |  |
| `kPi` | global | `plugin/ui/SpectrumAnalysis.cpp:12` | `constexpr double kPi = 3.14159265358979323846` |  |
| `waveNames` | global | `plugin/ui/Widgets.cpp:23` | `const char* const waveNames[15] = { "sine", "triangle", "half sine", "half tri", "2x sine", "2x tri", "2x half sine",...` |  |
| `APVTS` | alias | `plugin/ui/Widgets.h:19` | `using APVTS = juce::AudioProcessorValueTreeState` |  |
| `ROOT` | module constant | `tools/audit_modules.py:15` | `ROOT = Path(__file__).resolve().parents[1]` |  |
| `MODULES` | module constant | `tools/audit_modules.py:16` | `MODULES = ROOT / ".dev" / "modules"` |  |
| `PATCH` | module constant | `tools/audit_modules.py:17` | `PATCH = ROOT / "synth" / "patch.json"` |  |
| `PARAMS` | module constant | `tools/audit_modules.py:18` | `PARAMS = ROOT / "engine" / "gen" / "params.json"` |  |
| `NODES` | module constant | `tools/audit_modules.py:19` | `NODES = MODULES / "nodes"` |  |
| `MACROS` | module constant | `tools/audit_modules.py:20` | `MACROS = MODULES / "macros"` |  |
| `value` | module constant | `tools/audit_modules.py:37` | `value = value.get("target", "")` |  |
| `patch` | module constant | `tools/audit_modules.py:46` | `patch = load(PATCH)` |  |
| `used` | module constant | `tools/audit_modules.py:47` | `used: dict[str, list[dict]] = {}` |  |
| `macro` | module constant | `tools/audit_modules.py:52` | `macro = load(MACROS / "fm_op" / "macro.json")` |  |
| `missing` | module constant | `tools/audit_modules.py:56` | `missing = [kind for kind in used if not ( (NODES / kind / "node.json").exists() or (MACROS / kind / "macro.json").exi...` |  |
| `param_problems` | module constant | `tools/audit_modules.py:60` | `param_problems: list[str] = []` |  |
| `tables` | module constant | `tools/audit_modules.py:61` | `tables = set(patch.get("tables", {}))` |  |
| `spec` | module constant | `tools/audit_modules.py:63` | `spec = module_spec(kind)` |  |
| `declared` | module constant | `tools/audit_modules.py:64` | `declared = set(spec.get("params", {}))` |  |
| `map_problems` | module constant | `tools/audit_modules.py:73` | `map_problems: list[str] = []` |  |
| `node` | module constant | `tools/audit_modules.py:75` | `node = target_node(value)` |  |
| `params` | module constant | `tools/audit_modules.py:79` | `params = load(PARAMS)["params"]` |  |
| `names` | module constant | `tools/audit_modules.py:80` | `names = [p["name"] for p in params]` |  |
| `routing` | module constant | `tools/audit_modules.py:81` | `routing = ("channel.", "low.", "high.", "level.", "delay.")` |  |
| `part_block` | module constant | `tools/audit_modules.py:82` | `part_block = sum( name.endswith(".p0") and not name.startswith(routing) for name in names)` |  |
| `waveform_max` | module constant | `tools/audit_modules.py:84` | `waveform_max = max( (p["max"] for p in params if p["name"].startswith("op1_waveform")), default=-1)` |  |
| `spec_per_op` | module constant | `tools/audit_modules.py:87` | `spec_per_op = sum( 1 for name in names if name.startswith("op1_spec_") and name.endswith(".p0"))` |  |
| `spec_params` | module constant | `tools/audit_modules.py:90` | `spec_params = [p for p in params if p["name"].startswith("op1_spec_") and p["name"].endswith(".p0")]` |  |
| `spec_defaults` | module constant | `tools/audit_modules.py:92` | `spec_defaults = {p["name"]: p["default"] for p in spec_params if p["name"] in ( "op1_spec_a1.p0", "op1_spec_a2.p0", "...` |  |
| `expected_defaults` | module constant | `tools/audit_modules.py:98` | `expected_defaults = { "op1_spec_a1.p0": 99, "op1_spec_a2.p0": 0, "op1_spec_b1.p0": 99, "op1_spec_b2.p0": 45, "op1_spe...` |  |
| `nodes` | module constant | `tools/audit_modules.py:105` | `nodes = sorted(p.name for p in NODES.iterdir() if p.is_dir())` |  |
| `macros` | module constant | `tools/audit_modules.py:106` | `macros = sorted(p.name for p in MACROS.iterdir() if p.is_dir())` |  |
| `macro_params` | module constant | `tools/audit_modules.py:107` | `macro_params = len(macro.get("params", {}))` |  |
| `ROOT` | module constant | `tools/gen_code_docs.py:28` | `ROOT = Path(__file__).resolve().parents[1]` |  |
| `OUT` | module constant | `tools/gen_code_docs.py:29` | `OUT = ROOT / "docs" / "code" / "reference"` |  |
| `EXCLUDE` | module constant | `tools/gen_code_docs.py:30` | `EXCLUDE = re.compile( r"\\(build\|_deps\|\.git\|publish\|third-party-licenses\|__pycache__\|\.dev)(\\\|$)" )` |  |
| `SOURCE_EXT` | module constant | `tools/gen_code_docs.py:33` | `SOURCE_EXT = {".c", ".h", ".hpp", ".cpp", ".py", ".ps1", ".sh", ".cmake"}` |  |
| `TEXT_EXT` | module constant | `tools/gen_code_docs.py:34` | `TEXT_EXT = SOURCE_EXT \| {".json", ".md", ".txt", ".yml", ".yaml"}` |  |
| `CPP` | module constant | `tools/gen_code_docs.py:49` | `CPP = Language(tscpp.language())` |  |
| `C` | module constant | `tools/gen_code_docs.py:50` | `C = Language(tsc.language())` |  |
| `PY` | module constant | `tools/gen_code_docs.py:51` | `PY = Language(tspy.language())` |  |
| `t` | module constant | `tools/gen_code_docs.py:97` | `t = node.type` |  |
| `child` | module constant | `tools/gen_code_docs.py:110` | `child = node.child_by_field_name(field_name)` |  |
| `value` | module constant | `tools/gen_code_docs.py:112` | `value = name_of(child, src)` |  |
| `value` | module constant | `tools/gen_code_docs.py:116` | `value = name_of(child, src)` |  |
| `i` | module constant | `tools/gen_code_docs.py:124` | `i = start_line - 2` |  |
| `collected` | module constant | `tools/gen_code_docs.py:127` | `collected: list[str] = []` |  |
| `out` | module constant | `tools/gen_code_docs.py:142` | `out: list[str] = []` |  |
| `s` | module constant | `tools/gen_code_docs.py:144` | `s = line.strip()` |  |
| `s` | module constant | `tools/gen_code_docs.py:145` | `s = re.sub(r"^/\*+\|\*/$", "", s).strip()` |  |
| `s` | module constant | `tools/gen_code_docs.py:146` | `s = re.sub(r"^\*+\s?", "", s).strip()` |  |
| `s` | module constant | `tools/gen_code_docs.py:147` | `s = re.sub(r"^//+\s?", "", s).strip()` |  |
| `value` | module constant | `tools/gen_code_docs.py:154` | `value = re.sub(r"\s+", " ", value).strip()` |  |
| `value` | module constant | `tools/gen_code_docs.py:155` | `value = re.sub(r"\s*\{\s*$", "", value).strip()` |  |
| `parent` | module constant | `tools/gen_code_docs.py:168` | `parent = node.parent` |  |
| `owner` | module constant | `tools/gen_code_docs.py:179` | `owner = parent.parent` |  |
| `parent` | module constant | `tools/gen_code_docs.py:181` | `parent = owner.parent` |  |
| `parent` | module constant | `tools/gen_code_docs.py:193` | `parent = parent.parent` |  |
| `parent` | module constant | `tools/gen_code_docs.py:198` | `parent = node.parent` |  |
| `parent` | module constant | `tools/gen_code_docs.py:202` | `parent = parent.parent` |  |
| `raw` | module constant | `tools/gen_code_docs.py:207` | `raw = path.read_text(encoding="utf-8", errors="replace")` |  |
| `lines` | module constant | `tools/gen_code_docs.py:208` | `lines = raw.splitlines()` |  |
| `src` | module constant | `tools/gen_code_docs.py:209` | `src = raw.encode("utf-8")` |  |
| `tree` | module constant | `tools/gen_code_docs.py:210` | `tree = parser_for(path).parse(src)` |  |
| `rel` | module constant | `tools/gen_code_docs.py:211` | `rel = str(path.relative_to(ROOT)).replace("\\", "/")` |  |
| `functions` | module constant | `tools/gen_code_docs.py:212` | `functions: list[Function] = []` |  |
| `types` | module constant | `tools/gen_code_docs.py:213` | `types: list[Type] = []` |  |
| `symbols` | module constant | `tools/gen_code_docs.py:214` | `symbols: list[Symbol] = []` |  |
| `seen_functions` | module constant | `tools/gen_code_docs.py:216` | `seen_functions: set[tuple[str, int, str]] = set()` |  |
| `seen_types` | module constant | `tools/gen_code_docs.py:217` | `seen_types: set[tuple[str, int, str]] = set()` |  |
| `seen_symbols` | module constant | `tools/gen_code_docs.py:218` | `seen_symbols: set[tuple[str, int, str]] = set()` |  |
| `control_names` | module constant | `tools/gen_code_docs.py:219` | `control_names = { "if", "for", "while", "switch", "return", "sizeof", "static_assert", "alignas", "decltype", "catch"...` |  |
| `declarator` | module constant | `tools/gen_code_docs.py:225` | `declarator = node.child_by_field_name("declarator")` |  |
| `name` | module constant | `tools/gen_code_docs.py:226` | `name = name_of(declarator, src)` |  |
| `body` | module constant | `tools/gen_code_docs.py:229` | `body = node.child_by_field_name("body")` |  |
| `first` | module constant | `tools/gen_code_docs.py:230` | `first = node.start_point[0] + 1` |  |
| `last` | module constant | `tools/gen_code_docs.py:231` | `last = node.end_point[0] + 1` |  |
| `signature` | module constant | `tools/gen_code_docs.py:233` | `signature = clean_signature(text(node, src)[: body.start_byte - node.start_byte])` |  |
| `signature` | module constant | `tools/gen_code_docs.py:235` | `signature = clean_signature(text(node, src).rstrip(";"))` |  |
| `key` | module constant | `tools/gen_code_docs.py:236` | `key = (rel, first, name)` |  |
| `params` | module constant | `tools/gen_code_docs.py:240` | `params: list[str] = []` |  |
| `scope` | module constant | `tools/gen_code_docs.py:247` | `scope = class_scope(node, src)` |  |
| `description` | module constant | `tools/gen_code_docs.py:248` | `description = clean_comment(lines, first)` |  |
| `kind` | module constant | `tools/gen_code_docs.py:264` | `kind = { "class_specifier": "class", "struct_specifier": "struct", "union_specifier": "union", "enum_specifier": "enu...` |  |
| `name` | module constant | `tools/gen_code_docs.py:270` | `name = name_of(node.child_by_field_name("name"), src)` |  |
| `first` | module constant | `tools/gen_code_docs.py:273` | `first = node.start_point[0] + 1` |  |
| `key` | module constant | `tools/gen_code_docs.py:274` | `key = (rel, first, name)` |  |
| `body` | module constant | `tools/gen_code_docs.py:278` | `body = node.child_by_field_name("body")` |  |
| `kind_label` | module constant | `tools/gen_code_docs.py:279` | `kind_label = kind if body is not None else f"{kind} forward declaration"` |  |
| `fields` | module constant | `tools/gen_code_docs.py:280` | `fields: list[str] = []` |  |
| `methods` | module constant | `tools/gen_code_docs.py:281` | `methods: list[str] = []` |  |
| `body_node` | module constant | `tools/gen_code_docs.py:289` | `body_node = child.child_by_field_name("body")` |  |
| `raw` | module constant | `tools/gen_code_docs.py:291` | `raw = src[child.start_byte : body_node.start_byte].decode( "utf-8", "replace" )` |  |
| `raw` | module constant | `tools/gen_code_docs.py:295` | `raw = text(child, src)` |  |
| `t` | module constant | `tools/gen_code_docs.py:311` | `t = node.type` |  |
| `alias` | module constant | `tools/gen_code_docs.py:317` | `alias = name_of(node.child_by_field_name("declarator"), src)` |  |
| `underlying` | module constant | `tools/gen_code_docs.py:318` | `underlying = node.child_by_field_name("type")` |  |
| `first` | module constant | `tools/gen_code_docs.py:320` | `first = node.start_point[0] + 1` |  |
| `key` | module constant | `tools/gen_code_docs.py:321` | `key = (rel, first, alias)` |  |
| `name_node` | module constant | `tools/gen_code_docs.py:338` | `name_node = node.child_by_field_name("name")` |  |
| `name` | module constant | `tools/gen_code_docs.py:339` | `name = text(name_node, src) if name_node is not None else ""` |  |
| `first` | module constant | `tools/gen_code_docs.py:341` | `first = node.start_point[0] + 1` |  |
| `key` | module constant | `tools/gen_code_docs.py:342` | `key = (rel, first, name)` |  |
| `declarator` | module constant | `tools/gen_code_docs.py:356` | `declarator = node.child_by_field_name("declarator")` |  |
| `is_function` | module constant | `tools/gen_code_docs.py:357` | `is_function = declarator is not None and ( declarator.type == "function_declarator" or "function_declarator" in {c.ty...` |  |
| `name` | module constant | `tools/gen_code_docs.py:366` | `name = name_of(child.child_by_field_name("declarator"), src)` |  |
| `name` | module constant | `tools/gen_code_docs.py:368` | `name = name_of(child, src)` |  |
| `first` | module constant | `tools/gen_code_docs.py:373` | `first = node.start_point[0] + 1` |  |
| `key` | module constant | `tools/gen_code_docs.py:374` | `key = (rel, first, name)` |  |
| `declarator` | module constant | `tools/gen_code_docs.py:389` | `declarator = node.child_by_field_name("declarator")` |  |
| `is_function` | module constant | `tools/gen_code_docs.py:390` | `is_function = declarator is not None and ( declarator.type == "function_declarator" or "function_declarator" in {c.ty...` |  |
| `name` | module constant | `tools/gen_code_docs.py:397` | `name = name_of(node, src)` |  |
| `first` | module constant | `tools/gen_code_docs.py:399` | `first = node.start_point[0] + 1` |  |
| `key` | module constant | `tools/gen_code_docs.py:400` | `key = (rel, first, name)` |  |
| `raw` | module constant | `tools/gen_code_docs.py:421` | `raw = path.read_text(encoding="utf-8", errors="replace")` |  |
| `lines` | module constant | `tools/gen_code_docs.py:422` | `lines = raw.splitlines()` |  |
| `src` | module constant | `tools/gen_code_docs.py:423` | `src = raw.encode("utf-8")` |  |
| `tree` | module constant | `tools/gen_code_docs.py:424` | `tree = parser_for(path).parse(src)` |  |
| `rel` | module constant | `tools/gen_code_docs.py:425` | `rel = str(path.relative_to(ROOT)).replace("\\", "/")` |  |
| `functions` | module constant | `tools/gen_code_docs.py:426` | `functions: list[Function] = []` |  |
| `types` | module constant | `tools/gen_code_docs.py:427` | `types: list[Type] = []` |  |
| `symbols` | module constant | `tools/gen_code_docs.py:428` | `symbols: list[Symbol] = []` |  |
| `body` | module constant | `tools/gen_code_docs.py:431` | `body = node.child_by_field_name("body")` |  |
| `first` | module constant | `tools/gen_code_docs.py:436` | `first = child.children[0]` |  |
| `value` | module constant | `tools/gen_code_docs.py:438` | `value = text(first, src).strip("\"'")` |  |
| `name` | module constant | `tools/gen_code_docs.py:444` | `name = name_of(node.child_by_field_name("name"), src)` |  |
| `params` | module constant | `tools/gen_code_docs.py:446` | `params: list[str] = []` |  |
| `parameters` | module constant | `tools/gen_code_docs.py:447` | `parameters = node.child_by_field_name("parameters")` |  |
| `name` | module constant | `tools/gen_code_docs.py:466` | `name = name_of(node.child_by_field_name("name"), src)` |  |
| `left` | module constant | `tools/gen_code_docs.py:484` | `left = child.child_by_field_name("left")` |  |
| `files` | module constant | `tools/gen_code_docs.py:504` | `files: list[Path] = []` |  |
| `path` | module constant | `tools/gen_code_docs.py:508` | `path = Path(base, name)` |  |
| `raw` | module constant | `tools/gen_code_docs.py:521` | `raw = path.read_text(encoding="utf-8", errors="replace")` |  |
| `lines` | module constant | `tools/gen_code_docs.py:524` | `lines = raw.splitlines()` |  |
| `chunks` | module constant | `tools/gen_code_docs.py:530` | `chunks: list[str] = []` |  |
| `inside` | module constant | `tools/gen_code_docs.py:531` | `inside = False` |  |
| `saw_code` | module constant | `tools/gen_code_docs.py:532` | `saw_code = False` |  |
| `stripped` | module constant | `tools/gen_code_docs.py:534` | `stripped = line.strip()` |  |
| `body` | module constant | `tools/gen_code_docs.py:541` | `body = re.sub(r"^/\*+", "", stripped)` |  |
| `inside` | module constant | `tools/gen_code_docs.py:546` | `inside = True` |  |
| `saw_code` | module constant | `tools/gen_code_docs.py:554` | `saw_code = True` |  |
| `inside` | module constant | `tools/gen_code_docs.py:558` | `inside = False` |  |
| `cleaned` | module constant | `tools/gen_code_docs.py:563` | `cleaned = [chunk.strip() for chunk in chunks if chunk.strip()]` |  |
| `text` | module constant | `tools/gen_code_docs.py:566` | `text = " ".join(cleaned)` |  |
| `text` | module constant | `tools/gen_code_docs.py:567` | `text = re.sub(r"SPDX-License-Identifier:\s*\S+", " ", text)` |  |
| `text` | module constant | `tools/gen_code_docs.py:568` | `text = re.sub(r"Copyright\s*\(C\)[^.]*\.?", " ", text)` |  |
| `text` | module constant | `tools/gen_code_docs.py:569` | `text = re.sub(r"GENERATED by [^,]*,\s*do not edit[^.]*\.", " ", text, flags=re.I)` |  |
| `text` | module constant | `tools/gen_code_docs.py:570` | `text = re.sub(r"GENERATED by [^.]*\.", " ", text, flags=re.I)` |  |
| `text` | module constant | `tools/gen_code_docs.py:571` | `text = re.sub(r"\s+", " ", text).strip()` |  |
| `sentence` | module constant | `tools/gen_code_docs.py:573` | `sentence = sentence.strip().rstrip(".")` |  |
| `FILE_ROLES` | module constant | `tools/gen_code_docs.py:579` | `FILE_ROLES = { "engine/gen/dsp.h": "generated state struct, parameter table, slot table and engine API", "engine/gen/...` |  |
| `lines` | module constant | `tools/gen_code_docs.py:631` | `lines = [ "# File index", "", "Every tracked source and build file, with its line count and role.", "", "\| File \| Lin...` |  |
| `roles` | module constant | `tools/gen_code_docs.py:639` | `roles = [ ("engine/hal/", "fixed-point DSP, tables and spectral bank"), ("engine/runtime/", "preset records and the M...` |  |
| `rel` | module constant | `tools/gen_code_docs.py:659` | `rel = str(path.relative_to(ROOT)).replace("\\", "/")` |  |
| `count` | module constant | `tools/gen_code_docs.py:661` | `count = len(path.read_text(encoding="utf-8", errors="replace").splitlines())` |  |
| `count` | module constant | `tools/gen_code_docs.py:663` | `count = 0` |  |
| `lang` | module constant | `tools/gen_code_docs.py:664` | `lang = { ".c": "C", ".h": "C/C++ header", ".hpp": "C++ header", ".cpp": "C++", ".py": "Python", ".ps1": "PowerShell",...` |  |
| `lang` | module constant | `tools/gen_code_docs.py:679` | `lang = "CMake"` |  |
| `role` | module constant | `tools/gen_code_docs.py:680` | `role = "project source"` |  |
| `role` | module constant | `tools/gen_code_docs.py:683` | `role = value` |  |
| `summary` | module constant | `tools/gen_code_docs.py:685` | `summary = file_summary(path)` |  |
| `role` | module constant | `tools/gen_code_docs.py:687` | `role = FILE_ROLES[rel]` |  |
| `role` | module constant | `tools/gen_code_docs.py:689` | `role = summary` |  |
| `lines` | module constant | `tools/gen_code_docs.py:695` | `lines = [ "# Type, class, struct, union and enum index", "", "Every type definition found in the tracked C, C++ and P...` |  |
| `description` | module constant | `tools/gen_code_docs.py:706` | `description = t.description or CORE_TYPES.get(t.name, "")` |  |
| `description` | module constant | `tools/gen_code_docs.py:713` | `description = t.description or CORE_TYPES.get(t.name, "")` |  |
| `shown_fields` | module constant | `tools/gen_code_docs.py:724` | `shown_fields = t.fields[:40]` |  |
| `field_text` | module constant | `tools/gen_code_docs.py:727` | `field_text = field_text[:197] + "..."` |  |
| `hidden` | module constant | `tools/gen_code_docs.py:729` | `hidden = len(t.fields) - len(shown_fields)` |  |
| `shown_methods` | module constant | `tools/gen_code_docs.py:741` | `shown_methods = t.methods[:60]` |  |
| `method` | module constant | `tools/gen_code_docs.py:744` | `method = method[:197] + "..."` |  |
| `hidden` | module constant | `tools/gen_code_docs.py:746` | `hidden = len(t.methods) - len(shown_methods)` |  |
| `CORE_FUNCTIONS` | module constant | `tools/gen_code_docs.py:755` | `CORE_FUNCTIONS = { "PluginEditor": "Builds the editor, the header, the toolbar, the tabs and the first page, then sta...` |  |
| `CORE_TYPES` | module constant | `tools/gen_code_docs.py:853` | `CORE_TYPES = { "type_p_alpha_state": "The generated engine state. One instance per plugin instance.", "P1ParamSpec": ...` |  |
| `ACRONYMS` | module constant | `tools/gen_code_docs.py:878` | `ACRONYMS = { "pcm": "PCM", "midi": "MIDI", "ui": "UI", "dsp": "DSP", "crc": "CRC", "id": "id", "ids": "ids", "cpu": "...` |  |
| `name` | module constant | `tools/gen_code_docs.py:908` | `name = re.sub(r"^[~_]*(?:p1_\|type_p_alpha_)?", "", name)` |  |
| `pieces` | module constant | `tools/gen_code_docs.py:909` | `pieces = re.findall(r"[A-Z]+(?=[A-Z][a-z])\|[A-Z]?[a-z]+\|[A-Z]+\|\d+[a-z]*", name)` |  |
| `out` | module constant | `tools/gen_code_docs.py:910` | `out = [ACRONYMS.get(p.lower(), p.lower()) for p in pieces if p]` |  |
| `end` | module constant | `tools/gen_code_docs.py:916` | `end = min(f.end_line, len(lines))` |  |
| `i` | module constant | `tools/gen_code_docs.py:917` | `i = f.line - 1` |  |
| `stripped` | module constant | `tools/gen_code_docs.py:922` | `stripped = lines[i].strip()` |  |
| `block` | module constant | `tools/gen_code_docs.py:927` | `block = []` |  |
| `text` | module constant | `tools/gen_code_docs.py:931` | `text = " ".join(block)` |  |
| `block` | module constant | `tools/gen_code_docs.py:933` | `block = [stripped]` |  |
| `text` | module constant | `tools/gen_code_docs.py:937` | `text = " ".join(block)` |  |
| `text` | module constant | `tools/gen_code_docs.py:938` | `text = re.sub(r"/\*+\|\*/", " ", text)` |  |
| `text` | module constant | `tools/gen_code_docs.py:939` | `text = re.sub(r"^\*+\s?", "", text.strip())` |  |
| `text` | module constant | `tools/gen_code_docs.py:942` | `text = re.sub(r"\s+", " ", text).strip()` |  |
| `match` | module constant | `tools/gen_code_docs.py:943` | `match = re.match(r"([A-Z][^.]*\.)", text)` |  |
| `sentence` | module constant | `tools/gen_code_docs.py:945` | `sentence = match.group(1).strip()` |  |
| `name` | module constant | `tools/gen_code_docs.py:957` | `name = f.name` |  |
| `scope` | module constant | `tools/gen_code_docs.py:964` | `scope = f.scope` |  |
| `roles` | module constant | `tools/gen_code_docs.py:972` | `roles = { "note_to_inc": "Converts a MIDI note to a Q32 phase increment.", "type_p_alpha_param_slot": "Maps a manifes...` |  |
| `body` | module constant | `tools/gen_code_docs.py:1024` | `body = " ".join(lines[f.line - 1 : f.end_line])` |  |
| `match` | module constant | `tools/gen_code_docs.py:1025` | `match = re.search(r"withPart\(\"([^\"]+)\"", body)` |  |
| `match` | module constant | `tools/gen_code_docs.py:1028` | `match = re.search(r"opBase\(op,\s*\"([^\"]+)\"", body)` |  |
| `words` | module constant | `tools/gen_code_docs.py:1049` | `words = split_words(name)` |  |
| `rest` | module constant | `tools/gen_code_docs.py:1054` | `rest = split_words(name[5:])` |  |
| `rest` | module constant | `tools/gen_code_docs.py:1057` | `rest = split_words(name[7:])` |  |
| `rest` | module constant | `tools/gen_code_docs.py:1066` | `rest = split_words(name[4:])` |  |
| `rest` | module constant | `tools/gen_code_docs.py:1069` | `rest = split_words(name[5:])` |  |
| `overrides` | module constant | `tools/gen_code_docs.py:1086` | `overrides = { "paintButton": "Draws the button in its normal, highlighted and down states.", "paintListBoxItem": "Dra...` |  |
| `rest` | module constant | `tools/gen_code_docs.py:1112` | `rest = split_words(name[5:] if name.startswith("paint") else name[4:])` |  |
| `rest` | module constant | `tools/gen_code_docs.py:1115` | `rest = split_words(name[7:])` |  |
| `flags` | module constant | `tools/gen_code_docs.py:1117` | `flags = { "active", "compact", "selected", "visible", "enabled", "muted", "open", "closed", "bipolar", }` |  |
| `verb_prefixes` | module constant | `tools/gen_code_docs.py:1130` | `verb_prefixes = ( ("get", "Reads"), ("set", "Writes"), ("is", "Tests whether the"), ("has", "Tests whether the"), ("c...` |  |
| `rest` | module constant | `tools/gen_code_docs.py:1199` | `rest = split_words(name[len(prefix):])` |  |
| `roles` | module constant | `tools/gen_code_docs.py:1203` | `roles = { "midi_get": "The host hook that pops one raw MIDI message.", "synth_midi_feed": "Applies one MIDI message, ...` |  |
| `comment` | module constant | `tools/gen_code_docs.py:1212` | `comment = inner_comment(f, lines)` |  |
| `head` | module constant | `tools/gen_code_docs.py:1215` | `head = f.signature.split("(")[0]` |  |
| `returns` | module constant | `tools/gen_code_docs.py:1216` | `returns = head.replace(name, "").strip()` |  |
| `lines` | module constant | `tools/gen_code_docs.py:1225` | `lines = [ "# Function and method index", "", "Every function definition and declaration found in the tracked C, C++ a...` |  |
| `cache` | module constant | `tools/gen_code_docs.py:1237` | `cache: dict[str, list[str]] = {}` |  |
| `known` | module constant | `tools/gen_code_docs.py:1238` | `known: dict[tuple[str, str], str] = {}` |  |
| `ordered` | module constant | `tools/gen_code_docs.py:1239` | `ordered = sorted(functions, key=lambda x: (x.path, x.line, x.name))` |  |
| `source_lines` | module constant | `tools/gen_code_docs.py:1241` | `source_lines = cache.setdefault( f.path, (ROOT / f.path).read_text(encoding="utf-8", errors="replace").splitlines(), )` |  |
| `strong` | module constant | `tools/gen_code_docs.py:1245` | `strong = f.description or inner_comment(f, source_lines)` |  |
| `source_lines` | module constant | `tools/gen_code_docs.py:1253` | `source_lines = cache[f.path]` |  |
| `description` | module constant | `tools/gen_code_docs.py:1254` | `description = describe_function(f, source_lines, known)` |  |
| `description` | module constant | `tools/gen_code_docs.py:1256` | `description = description[0].upper() + description[1:]` |  |
| `signature` | module constant | `tools/gen_code_docs.py:1259` | `signature = f.signature` |  |
| `signature` | module constant | `tools/gen_code_docs.py:1261` | `signature = signature[:177] + "..."` |  |
| `current` | module constant | `tools/gen_code_docs.py:1268` | `current = ""` |  |
| `current` | module constant | `tools/gen_code_docs.py:1271` | `current = f.path` |  |
| `source_lines` | module constant | `tools/gen_code_docs.py:1277` | `source_lines = cache[f.path]` |  |
| `description` | module constant | `tools/gen_code_docs.py:1278` | `description = describe_function(f, source_lines, known)` |  |
| `lines` | module constant | `tools/gen_code_docs.py:1285` | `lines = [ "# Macro, global, alias and constant index", "", "Every macro, global variable, alias, using declaration an...` |  |
| `value` | module constant | `tools/gen_code_docs.py:1299` | `value = s.value` |  |
| `value` | module constant | `tools/gen_code_docs.py:1301` | `value = value[:117] + "..."` |  |
| `path` | module constant | `tools/gen_code_docs.py:1310` | `path = ROOT / "engine" / "gen" / "dsp.h"` |  |
| `raw` | module constant | `tools/gen_code_docs.py:1311` | `raw = path.read_text(encoding="utf-8")` |  |
| `start` | module constant | `tools/gen_code_docs.py:1312` | `start = raw.find("typedef struct type_p_alpha_state {")` |  |
| `end` | module constant | `tools/gen_code_docs.py:1313` | `end = raw.find("} type_p_alpha_state;", start)` |  |
| `body` | module constant | `tools/gen_code_docs.py:1314` | `body = raw[start:end]` |  |
| `out` | module constant | `tools/gen_code_docs.py:1315` | `out: list[tuple[str, str]] = []` |  |
| `line` | module constant | `tools/gen_code_docs.py:1317` | `line = line.strip()` |  |
| `m` | module constant | `tools/gen_code_docs.py:1318` | `m = re.match(r"^(.+?)\s+([A-Za-z_]\w*)(\[[^\]]+\])?;$", line)` |  |
| `lines` | module constant | `tools/gen_code_docs.py:1325` | `lines = [ "# `type_p_alpha_state` field index", "", "Every field of the generated engine state, in declaration order....` |  |
| `data` | module constant | `tools/gen_code_docs.py:1340` | `data = json.loads((ROOT / "engine" / "gen" / "params.json").read_text(encoding="utf-8"))` |  |
| `params` | module constant | `tools/gen_code_docs.py:1341` | `params = data["params"]` |  |
| `lines` | module constant | `tools/gen_code_docs.py:1342` | `lines = [ "# Parameter index", "", "Every manifest parameter. The part block has 994 values, mirrored for", "`.p0` an...` |  |
| `name` | module constant | `tools/gen_code_docs.py:1354` | `name = str(p["name"])` |  |
| `automatable` | module constant | `tools/gen_code_docs.py:1357` | `automatable = "yes" if p.get("automatable") else "no"` |  |
| `modulatable` | module constant | `tools/gen_code_docs.py:1358` | `modulatable = "yes" if p.get("modulatable") else "no"` |  |
| `path` | module constant | `tools/gen_code_docs.py:1368` | `path = ROOT / "engine" / "gen" / "type_p_alpha_dsp.c"` |  |
| `lines` | module constant | `tools/gen_code_docs.py:1371` | `lines = path.read_text(encoding="utf-8").splitlines()` |  |
| `out` | module constant | `tools/gen_code_docs.py:1372` | `out = [ "# Generated render walk", "", "The generated engine is one translation unit with a parameter table, a", "sta...` |  |
| `functions` | module constant | `tools/gen_code_docs.py:1387` | `functions = [ ("type_p_alpha_dsp_state_init", "initialise the state and apply the init table"), ("type_p_alpha_render...` |  |
| `start` | module constant | `tools/gen_code_docs.py:1415` | `start = next(i for i, l in enumerate(lines) if "type_p_alpha_render_voices_block" in l)` |  |
| `end` | module constant | `tools/gen_code_docs.py:1416` | `end = next(i for i, l in enumerate(lines) if "type_p_alpha_render_post_block" in l)` |  |
| `comments` | module constant | `tools/gen_code_docs.py:1417` | `comments: list[tuple[int, str]] = []` |  |
| `i` | module constant | `tools/gen_code_docs.py:1418` | `i = start` |  |
| `line` | module constant | `tools/gen_code_docs.py:1420` | `line = lines[i]` |  |
| `s` | module constant | `tools/gen_code_docs.py:1422` | `s = line.index("/*")` |  |
| `j` | module constant | `tools/gen_code_docs.py:1423` | `j = i` |  |
| `buf` | module constant | `tools/gen_code_docs.py:1424` | `buf: list[str] = []` |  |
| `l` | module constant | `tools/gen_code_docs.py:1426` | `l = lines[j]` |  |
| `begin` | module constant | `tools/gen_code_docs.py:1427` | `begin = s + 2 if j == i else 0` |  |
| `k` | module constant | `tools/gen_code_docs.py:1428` | `k = l.find("*/", begin)` |  |
| `txt` | module constant | `tools/gen_code_docs.py:1434` | `txt = " ".join(x.strip() for x in buf if x.strip())` |  |
| `i` | module constant | `tools/gen_code_docs.py:1437` | `i = j` |  |
| `seen` | module constant | `tools/gen_code_docs.py:1439` | `seen: set[str] = set()` |  |
| `counts` | module constant | `tools/gen_code_docs.py:1441` | `counts = Counter(t for _, t in comments)` |  |
| `nodes_dir` | module constant | `tools/gen_code_docs.py:1451` | `nodes_dir = ROOT / ".dev" / "modules"` |  |
| `lines` | module constant | `tools/gen_code_docs.py:1454` | `lines = [ "# Generator node catalogue", "", "The local `.dev` tree is the generator library and is not part of the pu...` |  |
| `data` | module constant | `tools/gen_code_docs.py:1466` | `data = json.loads(path.read_text(encoding="utf-8"))` |  |
| `ins` | module constant | `tools/gen_code_docs.py:1467` | `ins = ", ".join(data.get("ins", {}).keys())` |  |
| `outs` | module constant | `tools/gen_code_docs.py:1468` | `outs = ", ".join(data.get("outs", {}).keys())` |  |
| `data` | module constant | `tools/gen_code_docs.py:1475` | `data = json.loads(path.read_text(encoding="utf-8"))` |  |
| `exe_sources` | module constant | `tools/gen_code_docs.py:1484` | `exe_sources: dict[str, str] = {}` |  |
| `cmake` | module constant | `tools/gen_code_docs.py:1485` | `cmake = (ROOT / "plugin" / "CMakeLists.txt").read_text(encoding="utf-8")` |  |
| `juce` | module constant | `tools/gen_code_docs.py:1489` | `juce = (ROOT / "plugin" / "juce" / "CMakeLists.txt").read_text(encoding="utf-8")` |  |
| `manual` | module constant | `tools/gen_code_docs.py:1493` | `manual = { "linux_daws": "plugin/tests/linux_daws.sh", "plugin_smoke": "plugin/tests/plugin_smoke.cpp", "plugin_stabi...` |  |
| `tests` | module constant | `tools/gen_code_docs.py:1500` | `tests: list[tuple[str, str, str]] = []` |  |
| `lines` | module constant | `tools/gen_code_docs.py:1510` | `lines = [ "# Test index", "", "The CTest targets and their source files. The hand-written build chapter", "explains w...` |  |
| `source` | module constant | `tools/gen_code_docs.py:1521` | `source = "plugin/" + source` |  |
| `parser` | module constant | `tools/gen_code_docs.py:1527` | `parser = argparse.ArgumentParser()` |  |
| `args` | module constant | `tools/gen_code_docs.py:1529` | `args = parser.parse_args()` |  |
| `paths` | module constant | `tools/gen_code_docs.py:1531` | `paths = source_files(args.include_dev)` |  |
| `functions` | module constant | `tools/gen_code_docs.py:1532` | `functions: list[Function] = []` |  |
| `types` | module constant | `tools/gen_code_docs.py:1533` | `types: list[Type] = []` |  |
| `symbols` | module constant | `tools/gen_code_docs.py:1534` | `symbols: list[Symbol] = []` |  |
| `ROOT` | module constant | `tools/gen_math_tables.py:21` | `ROOT = Path(__file__).resolve().parents[1]` |  |
| `OUT` | module constant | `tools/gen_math_tables.py:22` | `OUT = ROOT / "engine" / "gen" / "math_tables.h"` |  |
| `OLD_RATE` | module constant | `tools/gen_math_tables.py:23` | `OLD_RATE = 44100.0` |  |
| `NEW_RATE` | module constant | `tools/gen_math_tables.py:24` | `NEW_RATE = 48000.0` |  |
| `BLOCK` | module constant | `tools/gen_math_tables.py:25` | `BLOCK = 32` |  |
| `FILTER_POS_MAX` | module constant | `tools/gen_math_tables.py:33` | `FILTER_POS_MAX = 55296` |  |
| `FILTER_OCTAVES` | module constant | `tools/gen_math_tables.py:34` | `FILTER_OCTAVES = 12.0` |  |
| `FILTER_HZ_MIN` | module constant | `tools/gen_math_tables.py:35` | `FILTER_HZ_MIN = 3.5` |  |
| `FILTER_STEPS` | module constant | `tools/gen_math_tables.py:36` | `FILTER_STEPS = 256` |  |
| `FILTER_COEFF_MAX` | module constant | `tools/gen_math_tables.py:37` | `FILTER_COEFF_MAX = 55296` |  |
| `v` | module constant | `tools/gen_math_tables.py:41` | `v = int(round(x * 32768.0))` |  |
| `v` | module constant | `tools/gen_math_tables.py:46` | `v = int(round(x * 65536.0))` |  |
| `lines` | module constant | `tools/gen_math_tables.py:51` | `lines = [f"static const {ctype} {name}[{len(values)}] = {{"]` |  |
| `row` | module constant | `tools/gen_math_tables.py:52` | `row: list[str] = []` |  |
| `row` | module constant | `tools/gen_math_tables.py:57` | `row = []` |  |
| `alpha` | module constant | `tools/gen_math_tables.py:80` | `alpha = min(1.0, max(0.0, coefficient / 65536.0))` |  |
| `beta` | module constant | `tools/gen_math_tables.py:81` | `beta = 1.0 - (1.0 - alpha) ** (old_rate / new_rate)` |  |
| `values` | module constant | `tools/gen_math_tables.py:87` | `values: list[int] = []` |  |
| `position` | module constant | `tools/gen_math_tables.py:89` | `position = i * FILTER_POS_MAX / FILTER_STEPS` |  |
| `hz` | module constant | `tools/gen_math_tables.py:90` | `hz = FILTER_HZ_MIN * 2.0 ** (position / FILTER_POS_MAX * FILTER_OCTAVES)` |  |
| `coefficient` | module constant | `tools/gen_math_tables.py:91` | `coefficient = 1.0 - math.exp(-2.0 * math.pi * hz / rate)` |  |
| `n` | module constant | `tools/gen_math_tables.py:97` | `n = 256` |  |
| `sin_q15` | module constant | `tools/gen_math_tables.py:98` | `sin_q15 = [q15(math.sin(math.pi / 2.0 * i / n)) for i in range(n)]` |  |
| `exp2_q16` | module constant | `tools/gen_math_tables.py:99` | `exp2_q16 = [q16(2.0 ** (-i / n)) for i in range(n)]` |  |
| `log2_q16` | module constant | `tools/gen_math_tables.py:100` | `log2_q16 = [q16(math.log2(1.0 + i / n)) for i in range(n)]` |  |
| `semi_q16` | module constant | `tools/gen_math_tables.py:101` | `semi_q16 = [q16(2.0 ** (i / 12.0)) for i in range(12)]` |  |
| `cent_q16` | module constant | `tools/gen_math_tables.py:102` | `cent_q16 = [q16(2.0 ** (i / 1200.0)) for i in range(100)]` |  |
| `detune_pitch` | module constant | `tools/gen_math_tables.py:103` | `detune_pitch = [ round(32768.0 * (2.0 ** (d / 1200.0) - 1.0)) for d in range(-50, 51) ]` |  |
| `rate_coeff` | module constant | `tools/gen_math_tables.py:106` | `rate_coeff = [keep_time(a, OLD_RATE, rate) for a in rate_base()]` |  |
| `rate_coeff_r3` | module constant | `tools/gen_math_tables.py:107` | `rate_coeff_r3 = [keep_time(a, OLD_RATE, rate) for a in rate_r3_base()]` |  |
| `level_gain` | module constant | `tools/gen_math_tables.py:108` | `level_gain = [0] + [ max(1, min(32767, round(32767.0 * 10.0 ** (-(99 - l) * 0.75 / 20.0)))) for l in range(1, 100) ]` |  |
| `tables` | module constant | `tools/gen_math_tables.py:127` | `tables = build(rate)` |  |
| `lines` | module constant | `tools/gen_math_tables.py:128` | `lines = [ "/* SPDX-License-Identifier: GPL-3.0-only", " * Copyright (C) 2026 P1Onken", " * GENERATED by tools/gen_mat...` |  |
| `parser` | module constant | `tools/gen_math_tables.py:162` | `parser = argparse.ArgumentParser()` |  |
| `args` | module constant | `tools/gen_math_tables.py:165` | `args = parser.parse_args()` |  |
| `ROOT` | module constant | `tools/gen_wavetable.py:29` | `ROOT = Path(__file__).resolve().parents[1]` |  |
| `OUT` | module constant | `tools/gen_wavetable.py:30` | `OUT = ROOT / "engine" / "gen" / "wavetable.h"` |  |
| `HARMONICS_OUT` | module constant | `tools/gen_wavetable.py:31` | `HARMONICS_OUT = ROOT / "engine" / "gen" / "wave_harmonics.h"` |  |
| `N` | module constant | `tools/gen_wavetable.py:32` | `N = 1024` |  |
| `MIP` | module constant | `tools/gen_wavetable.py:33` | `MIP = 9` |  |
| `WAVES` | module constant | `tools/gen_wavetable.py:34` | `WAVES = 14` |  |
| `BL_WAVES` | module constant | `tools/gen_wavetable.py:35` | `BL_WAVES = (10, 11, 12, 13)` |  |
| `v` | module constant | `tools/gen_wavetable.py:39` | `v = int(round(x * 32768.0))` |  |
| `v` | module constant | `tools/gen_wavetable.py:45` | `v = int(round(x))` |  |
| `SIN` | module constant | `tools/gen_wavetable.py:50` | `SIN = [q15(math.sin(math.pi / 2.0 * i / 256)) for i in range(256)]` |  |
| `q` | module constant | `tools/gen_wavetable.py:54` | `q = (phase >> 30) & 3` |  |
| `i` | module constant | `tools/gen_wavetable.py:55` | `i = (phase >> 22) & 255` |  |
| `v` | module constant | `tools/gen_wavetable.py:56` | `v = SIN[255 - i] if (q & 1) else SIN[i]` |  |
| `s` | module constant | `tools/gen_wavetable.py:66` | `s = sin_q15(phase)` |  |
| `a` | module constant | `tools/gen_wavetable.py:67` | `a = -s if s < 0 else s` |  |
| `half` | module constant | `tools/gen_wavetable.py:68` | `half = (phase >> 31) & 1` |  |
| `q` | module constant | `tools/gen_wavetable.py:69` | `q = (phase >> 30) & 3` |  |
| `s2` | module constant | `tools/gen_wavetable.py:81` | `s2 = sin_q15((phase << 1) & 0xFFFFFFFF)` |  |
| `s2` | module constant | `tools/gen_wavetable.py:84` | `s2 = sin_q15((phase << 1) & 0xFFFFFFFF)` |  |
| `s2` | module constant | `tools/gen_wavetable.py:87` | `s2 = sin_q15((phase << 1) & 0xFFFFFFFF)` |  |
| `p` | module constant | `tools/gen_wavetable.py:92` | `p = phase / 4294967296.0` |  |
| `q` | module constant | `tools/gen_wavetable.py:93` | `q = (p + 0.25) % 1.0` |  |
| `tri` | module constant | `tools/gen_wavetable.py:94` | `tri = 1.0 - 4.0 * abs(q - 0.5)` |  |
| `a` | module constant | `tools/gen_wavetable.py:113` | `a = [0.0] * (harm + 1)` |  |
| `b` | module constant | `tools/gen_wavetable.py:114` | `b = [0.0] * (harm + 1)` |  |
| `sa` | module constant | `tools/gen_wavetable.py:116` | `sa = sb = 0.0` |  |
| `ang` | module constant | `tools/gen_wavetable.py:118` | `ang = 2.0 * math.pi * k * i / N` |  |
| `v` | module constant | `tools/gen_wavetable.py:119` | `v = sin_wave(i << 22, ws)` |  |
| `out` | module constant | `tools/gen_wavetable.py:124` | `out: list[int] = []` |  |
| `ang` | module constant | `tools/gen_wavetable.py:126` | `ang = 2.0 * math.pi * i / N` |  |
| `total` | module constant | `tools/gen_wavetable.py:127` | `total = a[0] * 0.5` |  |
| `lines` | module constant | `tools/gen_wavetable.py:135` | `lines = [f"static const {ctype} {name}[{len(values)}] = {{"]` |  |
| `row` | module constant | `tools/gen_wavetable.py:136` | `row: list[str] = []` |  |
| `row` | module constant | `tools/gen_wavetable.py:141` | `row = []` |  |
| `wave` | module constant | `tools/gen_wavetable.py:149` | `wave: list[int] = []` |  |
| `base` | module constant | `tools/gen_wavetable.py:150` | `base: list[int] = []` |  |
| `rows` | module constant | `tools/gen_wavetable.py:163` | `rows: list[list[int]] = []` |  |
| `start` | module constant | `tools/gen_wavetable.py:165` | `start = base[ws] * N` |  |
| `table` | module constant | `tools/gen_wavetable.py:166` | `table = wave[start : start + N]` |  |
| `mags` | module constant | `tools/gen_wavetable.py:167` | `mags: list[float] = []` |  |
| `re` | module constant | `tools/gen_wavetable.py:169` | `re = 0.0` |  |
| `im` | module constant | `tools/gen_wavetable.py:170` | `im = 0.0` |  |
| `ang` | module constant | `tools/gen_wavetable.py:172` | `ang = 2.0 * math.pi * h * i / N` |  |
| `peak` | module constant | `tools/gen_wavetable.py:176` | `peak = max(mags) or 1.0` |  |
| `h` | module constant | `tools/gen_wavetable.py:182` | `h = [ "/* SPDX-License-Identifier: GPL-3.0-only", " * Copyright (C) 2026 P1Onken", " * GENERATED by tools/gen_wavetab...` |  |
| `h` | module constant | `tools/gen_wavetable.py:201` | `h = [ "/* SPDX-License-Identifier: GPL-3.0-only", " * Copyright (C) 2026 P1Onken", " * GENERATED by tools/gen_wavetab...` |  |
| `parser` | module constant | `tools/gen_wavetable.py:224` | `parser = argparse.ArgumentParser()` |  |
| `args` | module constant | `tools/gen_wavetable.py:227` | `args = parser.parse_args()` |  |
| `rows` | module constant | `tools/gen_wavetable.py:231` | `rows = harmonics(wave, base)` |  |
