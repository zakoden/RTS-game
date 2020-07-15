#include "abstract_unit.h"

void AbstractUnit::SetPosition(const Cell& position) { position_ = position; }
void AbstractUnit::SetBehavior(Behavior* behavior) { behavior_.reset(behavior); }

std::string AbstractUnit::GetName() const { return name_; }

void AbstractUnit::AddEffect(Effect effect) { effects_.set(effect); }

void AbstractUnit::RemoveEffect(Effect effect) { effects_.reset(effect); }

bool AbstractUnit::HasEffect(Effect effect) const { return effects_.test(effect); }