#pragma once

#define SPV_ENABLE_UTILITY_CODE
#include <spirv.hpp11>

namespace spvgentwo
{
	constexpr unsigned int makeGeneratorId(unsigned short _gen, unsigned short _ver) { return _gen << 16 | _ver; }
	constexpr unsigned int makeVersion(unsigned char _major, unsigned char _minor) { return _major << 16 | (_minor << 8); }
	constexpr unsigned char getMajorVersion(unsigned int _version) { return (_version & 0x00FF0000) >> 16; }
	constexpr unsigned char getMinorVersion(unsigned int _version) { return (_version & 0x0000FF00) >> 8; }

	// Sampled indicates whether or not this image will be accessed in combination with a sampler, and must be one of the following values:
	enum class SamplerImageAccess : unsigned int
	{
		Unknown = 0, // indicates this is only known at run time, not at compile time
		Sampled = 1, // indicates will be used with sampler
		Storage = 2  // indicates will be used without a sampler (a storage image)
	};

	enum class ConstantSamplerCoordMode : unsigned int
	{
		UnNormalized = 0,
		Normalized = 1
	};

	enum class MemoryOperands : unsigned int
	{
		None = 0x0,
		Volatile = 0x1,
		Aligned = 0x2,
		Nontemporal = 0x4,
		MakePointerAvailableKHR = 0x8,
		MakePointerVisibleKHR = 0x10,
		NonPrivatePointerKHR = 0x20
	};

	constexpr bool isTypeOp(const spv::Op _type)
	{		
		if ((_type >= spv::Op::OpTypeVoid && _type <= spv::Op::OpTypeForwardPointer) || // basic types
			(_type >= spv::Op::OpTypeVmeImageINTEL && _type <= spv::Op::OpTypeAvcSicResultINTEL) // intel ext
			)
		{
			return true;
		}

		// special cases and small extensions
		switch (_type)
		{
		case spv::Op::OpTypePipeStorage:
		case spv::Op::OpTypeNamedBarrier:
		case spv::Op::OpTypeAccelerationStructureNV:
		case spv::Op::OpTypeCooperativeMatrixNV:
			return true;
		default:
			return false;
		}
	}

	constexpr size_t wordCount(const size_t _byteCount)
	{
		return (_byteCount / sizeof(spv::Id)) + (_byteCount % sizeof(spv::Id) != 0u ? 1u : 0u);
	}

	inline bool hasResultId(const spv::Op _operation)
	{
		bool res = false, type = false;
		spv::HasResultAndType(_operation, &res, &type);
		return res;
	}

	inline bool hasResultTypeId(const spv::Op _operation)
	{
		bool res = false, type = false;
		spv::HasResultAndType(_operation, &res, &type);
		return type;
	}

	inline bool hasResultAndTypeId(const spv::Op _operation)
	{
		bool res = false, type = false;
		spv::HasResultAndType(_operation, &res, &type);
		return res && type;
	}

	// returns either spv::OpExecutionMode or spv::OpExecutionModeId based on input mode
	constexpr spv::Op getExecutionModeOp(const spv::ExecutionMode _mode)
	{
		switch (_mode)
		{
		case spv::ExecutionMode::SubgroupsPerWorkgroupId:
		case spv::ExecutionMode::LocalSizeId:
		case spv::ExecutionMode::LocalSizeHintId:
			return spv::Op::OpExecutionModeId;
		default:
			return spv::Op::OpExecutionMode;
		}
	}

	constexpr bool isTerminatorOp(const spv::Op _instr)
	{
		switch (_instr)
		{
		case spv::Op::OpBranch:
		case spv::Op::OpBranchConditional:
		case spv::Op::OpSwitch:
		case spv::Op::OpReturn:
		case spv::Op::OpReturnValue:
		case spv::Op::OpKill:
		case spv::Op::OpUnreachable:
			return true;
		default:
			return false;
		}
	}

	// starts with "OpConstant" 
	constexpr bool isConstantOp(const spv::Op _instr)
	{
		switch (_instr)
		{
		case spv::Op::OpConstantTrue:
		case spv::Op::OpConstantFalse:
		case spv::Op::OpConstant:
		case spv::Op::OpConstantComposite:
		case spv::Op::OpConstantSampler:
		case spv::Op::OpConstantNull:
		case spv::Op::OpConstantPipeStorage:
			return true;
		default:
			return false;
		}
	}

	// starts with "OpSpec" 
	constexpr bool isSpecOp(const spv::Op _instr)
	{
		switch (_instr)
		{
		case spv::Op::OpSpecConstant:
		case spv::Op::OpSpecConstantTrue:
		case spv::Op::OpSpecConstantFalse:
		case spv::Op::OpSpecConstantComposite:
		case spv::Op::OpSpecConstantOp:
			return true;
		default:
			return false;
		}
	}

	// Instructions that start "OpConstant" or "OpSpec"
	constexpr bool isSpecOrConstantOp(const spv::Op _instr)
	{
		return isConstantOp(_instr) || isSpecOp(_instr);
	}

	// spir-v specific traits
	namespace traits
	{
		template <class T> constexpr bool is_primitive_type_v = false;
		template <>	constexpr bool is_primitive_type_v<bool> = true;
		template <>	constexpr bool is_primitive_type_v<short> = true;
		template <>	constexpr bool is_primitive_type_v<unsigned short> = true;
		template <>	constexpr bool is_primitive_type_v<int> = true;
		template <>	constexpr bool is_primitive_type_v<unsigned int> = true;
		template <>	constexpr bool is_primitive_type_v<long> = true;
		template <>	constexpr bool is_primitive_type_v<unsigned long> = true;
		template <>	constexpr bool is_primitive_type_v<long long> = true;
		template <>	constexpr bool is_primitive_type_v<unsigned long long> = true;
		template <>	constexpr bool is_primitive_type_v<float> = true;
		template <>	constexpr bool is_primitive_type_v<double> = true;
	}
} //!spvgentwo