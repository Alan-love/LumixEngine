#pragma once

#include "condition.h"
#include "engine/flag_set.h"
#include "engine/resource.h"
#include "engine/stream.h"

namespace Lumix {

struct LocalRigidTransform;
struct Pose;

namespace Anim {

struct GroupNode;
struct RuntimeContext;

class Controller final : public Resource {
public:
	Controller(const Path& path, ResourceManager& resource_manager, IAllocator& allocator);

	void serialize(OutputMemoryStream& stream);
	bool deserialize(InputMemoryStream& stream);

	RuntimeContext* createRuntime(u32 anim_set);
	void destroyRuntime(RuntimeContext& ctx);
	void update(RuntimeContext& ctx, Ref<LocalRigidTransform> root_motion);
	void getPose(RuntimeContext& ctx, Ref<struct Pose> pose);
	void initEmpty();

	ResourceType getType() const override { return TYPE; }
	static const ResourceType TYPE;

	struct AnimationEntry {
		u32 set;
		u32 slot;
		Animation* animation;
	};

	IAllocator& m_allocator;
	GroupNode* m_root = nullptr;
	Array<AnimationEntry> m_animation_entries;
	Array<String> m_animation_slots;
	InputDecl m_inputs;
	enum class Flags : u32 {
		USE_ROOT_MOTION = 1 << 0
	};
	FlagSet<Flags, u32> m_flags;
	struct IK {
		enum { MAX_BONES_COUNT = 8 };
		u16 max_iterations = 5;
		u16 bones_count = 4;
		u32 bones[MAX_BONES_COUNT];
	} m_ik[4];
	u32 m_ik_count = 0;

private:
	void unload() override;
	bool load(u64 size, const u8* mem) override;
};

} // ns Anim
} // ns Lumix