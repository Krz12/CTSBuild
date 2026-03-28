#pragma once
struct entity_id {
    int index;
    int generation;

    bool operator==(const entity_id& other) const {
        return index == other.index && generation == other.generation;
    }
};