#include "ssl/world.h"
/** TODO:
 *  - Add robot_count conf (pworld)
 *  - Add ball radius, ball mass
 *
 */
World::World() {
    pworld = new PWorld(0.05, 9.81, getConf().game.robot_count);
    pball = new PBall(0, 0, 0.5, getConf().ball.radius, getConf().ball.mass, 1,
                      0.7, 0),
    pground = new PGround(
        getConf().field.radius, getConf().field.length, getConf().field.width,
        getConf().field.penalty_depth, getConf().field.penalty_width,
        getConf().field.penalty_point, getConf().field.line_width, 0);
    pray = new PRay(50);

    // Bouding walls
    const double thick = getConf().field.wall_thickness;
    const double increment =
        getConf().field.margin + getConf().field.referee_margin + thick / 2;
    const double pos_x = getConf().field.length / 2.0 + increment;
    const double pos_y = getConf().field.width / 2.0 + increment;
    const double pos_z = 0.0;
    const double siz_x = 2.0 * pos_x;
    const double siz_y = 2.0 * pos_y;
    const double siz_z = 0.4;
    const double tone = 1.0;

    walls[0] = new PFixedBox(thick / 2, pos_y, pos_z, siz_x, thick, siz_z, tone,
                             tone, tone);

    walls[1] = new PFixedBox(-thick / 2, -pos_y, pos_z, siz_x, thick, siz_z,
                             tone, tone, tone);

    walls[2] = new PFixedBox(pos_x, -thick / 2, pos_z, thick, siz_y, siz_z,
                             tone, tone, tone);

    walls[3] = new PFixedBox(-pos_x, thick / 2, pos_z, thick, siz_y, siz_z,
                             tone, tone, tone);

    // Goal walls

    const double gthick = getConf().field.goal_thickness;
    const double gpos_x =
        (getConf().field.length + gthick) / 2.0 + getConf().field.goal_depth;
    const double gpos_y = (getConf().field.goal_width + gthick) / 2.0;
    const double gpos_z = getConf().field.goal_height / 2.0;
    const double gsiz_x = getConf().field.goal_depth + gthick;
    const double gsiz_y = getConf().field.goal_width;
    const double gsiz_z = getConf().field.goal_height;
    const double gpos2_x = (getConf().field.length + gsiz_x) / 2.0;

    walls[4] = new PFixedBox(gpos_x, 0.0, gpos_z, gthick, gsiz_y, gsiz_z, tone,
                             tone, tone);

    walls[5] = new PFixedBox(gpos2_x, -gpos_y, gpos_z, gsiz_x, gthick, gsiz_z,
                             tone, tone, tone);

    walls[6] = new PFixedBox(gpos2_x, gpos_y, gpos_z, gsiz_x, gthick, gsiz_z,
                             tone, tone, tone);

    walls[7] = new PFixedBox(-gpos_x, 0.0, gpos_z, gthick, gsiz_y, gsiz_z, tone,
                             tone, tone);

    walls[8] = new PFixedBox(-gpos2_x, -gpos_y, gpos_z, gsiz_x, gthick, gsiz_z,
                             tone, tone, tone);

    walls[9] = new PFixedBox(-gpos2_x, gpos_y, gpos_z, gsiz_x, gthick, gsiz_z,
                             tone, tone, tone);

    pworld->addObject(pground);
    pworld->addObject(pball);
    for (auto& wall : walls) pworld->addObject(wall);
}