entities = {}


def add_entity(entity_id, entity):
    entities[entity_id] = entity

def get_entity(entity_id):
    if entity_id in entities:
        return entities[entity_id]
    else:
        return None


def remove_entity(entity_id):
    if entities is not None:
        entity = entities[entity_id]
        del entities[entity_id]
        del entity
