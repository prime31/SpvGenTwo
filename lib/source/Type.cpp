#include "Type.h"

spvgentwo::Type::Type(IAllocator* _pAllocator, Type* _pParent) :
	m_pParent(_pParent),
	m_subTypes(_pAllocator)
{
}

spvgentwo::Type::Type(IAllocator* _pAllocator, const Type& _subType, const spv::Op _baseType) :
	m_Type(_baseType),
	m_pParent(nullptr),
	m_subTypes(_pAllocator)
{
	m_subTypes.emplace_back(_subType);
}

spvgentwo::Type::Type(IAllocator* _pAllocator, Type&& _subType, const spv::Op _baseType) :
	m_Type(_baseType),
	m_pParent(nullptr),
	m_subTypes(_pAllocator)
{
	m_subTypes.emplace_back(stdrep::move(_subType));
}

spvgentwo::Type::Type(Type&& _other) noexcept:
	m_Type(_other.m_Type),
	m_IntWidth(_other.m_IntWidth),
	m_IntSign(_other.m_IntSign),
	m_StorageClass(_other.m_StorageClass),
	m_ImgDimension(_other.m_ImgDimension),
	m_ImgArray(_other.m_ImgArray),
	m_ImgMultiSampled(_other.m_ImgMultiSampled),
	m_ImgSamplerAccess(_other.m_ImgSamplerAccess),
	m_ImgFormat(_other.m_ImgFormat),
	m_AccessQualifier(_other.m_AccessQualifier),
	m_subTypes(stdrep::move(_other.m_subTypes))
{
	for (Type& t : m_subTypes)
	{
		t.m_pParent = this;
	}
}

spvgentwo::Type::Type(const Type& _other) : 
	m_Type(_other.m_Type),
	m_IntWidth(_other.m_IntWidth),
	m_IntSign(_other.m_IntSign),
	m_StorageClass(_other.m_StorageClass),
	m_ImgDimension(_other.m_ImgDimension),
	m_ImgArray(_other.m_ImgArray),
	m_ImgMultiSampled(_other.m_ImgMultiSampled),
	m_ImgSamplerAccess(_other.m_ImgSamplerAccess),
	m_ImgFormat(_other.m_ImgFormat),
	m_AccessQualifier(_other.m_AccessQualifier),
	m_subTypes(_other.m_subTypes)
{
	for (Type& t : m_subTypes)
	{
		t.m_pParent = this;
	}
}

spvgentwo::Type::~Type()
{
}

spvgentwo::Type& spvgentwo::Type::operator=(Type&& _other) noexcept
{
	if (this == &_other) return *this;

	m_subTypes = stdrep::move(_other.m_subTypes);

	m_Type = _other.m_Type;
	m_IntSign = _other.m_IntSign;
	m_IntWidth = _other.m_IntWidth;
	m_ImgDimension = _other.m_ImgDimension;
	m_ImgArray = _other.m_ImgArray;
	m_ImgMultiSampled = _other.m_ImgMultiSampled;
	m_ImgSamplerAccess = _other.m_ImgSamplerAccess;
	m_ImgFormat = _other.m_ImgFormat;
	m_StorageClass = _other.m_StorageClass;
	m_AccessQualifier = _other.m_AccessQualifier;

	for (Type& t : m_subTypes)
	{
		t.m_pParent = this;
	}

	return *this;
}
spvgentwo::Type& spvgentwo::Type::operator=(const Type& _other)
{
	if (this == &_other) return *this;
	m_subTypes = _other.m_subTypes;

	m_Type = _other.m_Type;
	m_IntSign = _other.m_IntSign;
	m_IntWidth = _other.m_IntWidth;
	m_ImgDimension = _other.m_ImgDimension;
	m_ImgArray = _other.m_ImgArray;
	m_ImgMultiSampled = _other.m_ImgMultiSampled;
	m_ImgSamplerAccess = _other.m_ImgSamplerAccess;
	m_ImgFormat = _other.m_ImgFormat;
	m_StorageClass = _other.m_StorageClass;
	m_AccessQualifier = _other.m_AccessQualifier;

	for (Type& t : m_subTypes)
	{
		t.m_pParent = this;
	}

	return *this;
}

bool spvgentwo::Type::operator==(const Type& _other) const
{
	return
		m_Type == _other.m_Type &&
		m_IntSign == _other.m_IntSign &&
		m_IntWidth == _other.m_IntWidth &&
		m_ImgDimension == _other.m_ImgDimension &&
		m_ImgArray == _other.m_ImgArray &&
		m_ImgMultiSampled == _other.m_ImgMultiSampled &&
		m_ImgSamplerAccess ==_other.m_ImgSamplerAccess &&
		m_ImgFormat == _other.m_ImgFormat &&
		m_StorageClass == _other.m_StorageClass &&
		m_AccessQualifier == _other.m_AccessQualifier &&
		m_subTypes == _other.m_subTypes;
}

const spvgentwo::Type& spvgentwo::Type::getBaseType() const
{
	if (m_subTypes.empty())
	{
		return *this;
	}

	return front().getBaseType();
}

spvgentwo::Type& spvgentwo::Type::getBaseType()
{
	if (m_subTypes.empty())
	{
		return *this;
	}

	return front().getBaseType();
}

spv::Op spvgentwo::Type::getBaseTypeOp() const
{
	return getBaseType().m_Type;
}

bool spvgentwo::Type::isBaseTypeOf(const spv::Op _type) const
{
	return getBaseType().getType() == _type;
}

bool spvgentwo::Type::hasSameBase(const Type& _other, const bool _onlyCheckTyeOp) const
{
	const Type& lBase = getBaseType();
	const Type& rBase = _other.getBaseType();

	return _onlyCheckTyeOp ? lBase.m_Type == rBase.m_Type : lBase == rBase;
}

void spvgentwo::Type::setType(const spv::Op _type)
{
	if (isTypeOp(_type))
	{
		m_Type = _type;
	}
}

void spvgentwo::Type::reset()
{
	m_Type = spv::Op::OpTypeVoid;
	m_subTypes.clear();
}

spvgentwo::Type& spvgentwo::Type::Void()
{
	m_Type = spv::Op::OpTypeVoid;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::Bool()
{
	m_Type = spv::Op::OpTypeBool;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::Int(const unsigned int _bits, const bool _sign)
{
	m_Type = spv::Op::OpTypeInt;
	m_IntWidth = _bits;
	m_IntSign = _sign;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::Float(const unsigned int _bits)
{
	m_Type = spv::Op::OpTypeFloat;
	m_FloatWidth = _bits;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::Scalar(const spv::Op _base, const unsigned int _bits, const bool _sign)
{
	m_Type = _base;
	m_FloatWidth = _bits;
	m_IntSign = _sign;

	return *this;
}

spvgentwo::Type& spvgentwo::Type::Struct()
{
	m_Type = spv::Op::OpTypeStruct;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::Array(const unsigned int _elements, const Type* _pElementType)
{
	m_Type = spv::Op::OpTypeArray;
	m_ArrayLength = _elements;

	if(_pElementType != nullptr)
	{
		m_subTypes.emplace_back(*_pElementType);
	}

	return *this;
}

spvgentwo::Type& spvgentwo::Type::RuntimeArray(const Type* _elementType)
{
	m_Type = spv::Op::OpTypeRuntimeArray;

	if (_elementType != nullptr)
	{
		m_subTypes.emplace_back(*_elementType);
	}

	return *this;
}

spvgentwo::Type& spvgentwo::Type::Function()
{
	m_Type = spv::Op::OpTypeFunction;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::Pointer(const spv::StorageClass _storageClass)
{
	m_Type = spv::Op::OpTypePointer;
	m_StorageClass = _storageClass;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::ForwardPointer(const spv::StorageClass _storageClass)
{
	m_Type = spv::Op::OpTypeForwardPointer;
	m_StorageClass = _storageClass;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::Sampler()
{
	m_Type = spv::Op::OpTypeSampler;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::Image(const Type* _pSampledType, const spv::Dim _dim, const unsigned int _depth, const bool _array, const bool _multiSampled, const SamplerImageAccess _sampled, const spv::ImageFormat _format, const spv::AccessQualifier _access)
{
	m_Type = spv::Op::OpTypeImage;

	if (_pSampledType != nullptr)
	{
		m_subTypes.emplace_back(*_pSampledType);
	}

	m_ImgDimension = _dim;
	m_ImgDepth = _depth;
	m_ImgArray = _array;
	m_ImgMultiSampled = _multiSampled;
	m_ImgSamplerAccess = _sampled;
	m_ImgFormat = _format;
	m_AccessQualifier = _access;

	return *this;
}

spvgentwo::Type& spvgentwo::Type::SampledImage(const Type* _imageType)
{
	m_Type = spv::Op::OpTypeSampledImage;

	if (_imageType != nullptr)
	{
		m_subTypes.emplace_back(*_imageType);
	}

	return *this;
}

spvgentwo::Type& spvgentwo::Type::SampledImage(const dyn_image_t* _imageType)
{
	m_Type = spv::Op::OpTypeSampledImage;

	if (_imageType != nullptr)
	{
		Member().fundamental<dyn_image_t>(_imageType);
	}

	return *this;
}

spvgentwo::Type& spvgentwo::Type::Event()
{
	m_Type = spv::Op::OpTypeEvent;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::DeviceEvent()
{
	m_Type = spv::Op::OpTypeDeviceEvent;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::ReserveId()
{
	m_Type = spv::Op::OpTypeReserveId;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::Queue()
{
	m_Type = spv::Op::OpTypeQueue;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::PipeStorage()
{
	m_Type = spv::Op::OpTypePipeStorage;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::NamedBarrier()
{
	m_Type = spv::Op::OpTypeNamedBarrier;
	return *this;
}

spvgentwo::Type& spvgentwo::Type::Vector(unsigned int _elements, const Type* _elementType)
{
	m_Type = spv::Op::OpTypeVector;
	m_VecComponentCount = _elements;

	if (_elementType != nullptr)
	{
		m_subTypes.emplace_back(*_elementType);
	}

	return *this;
}

spvgentwo::Type& spvgentwo::Type::Matrix(unsigned int _columns, const Type* _columnType)
{
	m_Type = spv::Op::OpTypeMatrix;
	m_MatColumnCount = _columns; // length of the row

	if (_columnType != nullptr)
	{
		m_subTypes.emplace_back(*_columnType);
	}

	return *this;
}

spvgentwo::Type& spvgentwo::Type::Member()
{
	return m_subTypes.emplace_back(m_subTypes.getAllocator(), this);
}

spvgentwo::Type& spvgentwo::Type::Parent()
{
	if (m_pParent == nullptr) return *this;
	return *m_pParent;
}

spvgentwo::Type& spvgentwo::Type::Top()
{
	Type* parent = this;
	while (true) {
		if (parent->m_pParent == nullptr) { return *parent; }
		parent = parent->m_pParent;
	}
}

spvgentwo::Type spvgentwo::Type::wrap(const spv::Op _baseType) const
{
	 return Type(m_subTypes.getAllocator(), *this, _baseType);
}

spvgentwo::Type spvgentwo::Type::moveWrap(const spv::Op _baseType)
{
	return Type(m_subTypes.getAllocator(), stdrep::move(*this), _baseType);
}