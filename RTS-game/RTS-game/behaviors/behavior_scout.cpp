#include "behavior_scout.h"

#include "../map/grid_function.h"

#include "../adapter.h"

BehaviorScout::BehaviorScout(MovableUnit* unit, GameMap* game_map, MapPoint center)
	: unit_(unit)
	, game_map_(game_map)
	, center_(center) {}

/* It finds the most suitable point to uncover for the scout
Currently, it assings to each cell a score = 
COEF_DISTANCE_FROM_BASE * (distance from cell to the nearest base)
+ COEF_DISTANCE_FROM_UNIT * (distance from cell to this unit)
and then unit goes to a cell with the highest score
*/
void BehaviorScout::DoAction() {
	if (target_.x == -1 ||
		game_map_->IsCellUncovered(adapter::UnitToMapCoordinates(target_), unit_->GetPlayer())) {  // No target
		const float COEF_DISTANCE_FROM_BASE = -1, COEF_DISTANCE_FROM_UNIT = -3, COEF_DISTANCE_FROM_CENTER = -2;
		MapPoint unit = adapter::UnitToMapCoordinates(target_);
		float target_score = -1000000.0f;

		for (uint32_t i = 0; i < game_map_->GetHeight(); ++i) {
			for (uint32_t j = 0; j < game_map_->GetWidth(); ++j) {
				if (!game_map_->IsCellUncovered({j, i}, unit_->GetPlayer())) {
					float score =
						COEF_DISTANCE_FROM_BASE * game_map_->GetDistanceToBase(unit_->GetPlayer())[i][j]
						+ COEF_DISTANCE_FROM_UNIT * grid_function::Distance(unit, { j, i })
						+ COEF_DISTANCE_FROM_CENTER * grid_function::Distance(center_, { j, i });
					if (target_score < score) {
						target_score = score;
						target_ = adapter::MapToUnitCoordinates({ j, i });
					}
				}
			}
		}
		//std::cout << target_.x << " " << target_.y << " " << target_score << std::endl;
	}
	unit_->SetVector(target_.x - ((AbstractImmovableUnit*)unit_)->GetX(), target_.y - (((AbstractImmovableUnit*)unit_)->GetY()));
	unit_->VectorApply();
}
