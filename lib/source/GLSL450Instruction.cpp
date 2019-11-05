#include "GLSL450Instruction.h"
#include "Module.h"

spvgentwo::Instruction* spvgentwo::GLSL450Intruction::scalarOrFloatVec1(const extinstr::GLSLstd450 _op, Instruction* _pFloat, const bool _no64Bit)
{
	Instruction* _pTypeInstr = _pFloat->getTypeInst();
	const Type* pType = _pTypeInstr->getType();

	if ((pType->isFloat() || pType->isVectorOfFloat()) && (_no64Bit ? pType->getFloatWidth() <= 32u : true))
	{
		return opExtInst(_pTypeInstr, ExtName, _op, _pFloat);
	}

	getModule()->logError("Operands of scalarOrFloatVec1 opeartion are not vector or scalar of float");

	return nullptr;
}

spvgentwo::Instruction* spvgentwo::GLSL450Intruction::opCross(Instruction* _pLeft, Instruction* _pRight)
{
	Instruction* _pLeftTypeInstr = _pLeft->getTypeInst();
	Instruction* _pRightTypeInstr = _pRight->getTypeInst();

	const Type* pType = _pLeftTypeInstr->getType();
	if (_pLeftTypeInstr == _pRightTypeInstr && pType->isVectorOfScalar(3u))
	{
		return opExtInst(_pLeftTypeInstr, ExtName, extinstr::GLSLstd450::GLSLstd450Cross, _pLeft, _pRight);	
	}
	
	getModule()->logError("Operands of opCross are not scalar vector of length 3");

	return nullptr;
}

spvgentwo::Instruction* spvgentwo::GLSL450Intruction::opDeterminant(Instruction* _pMatrix)
{
	const Type* pType = _pMatrix->getType();

	if (pType->isSqareMatrix())
	{
		Instruction* pResultType = getModule()->addType(pType->getBaseType());
		return opExtInst(pResultType, ExtName, extinstr::GLSLstd450::GLSLstd450Determinant, _pMatrix);
	}

	getModule()->logError("Operand of opDeterminant is not a square matirx");

	return nullptr;
}

spvgentwo::Instruction* spvgentwo::GLSL450Intruction::opMatrixInverse(Instruction* _pMatrix)
{
	Instruction* _pTypeInstr = _pMatrix->getTypeInst();
	const Type* pType = _pTypeInstr->getType();

	if (pType->isSqareMatrix())
	{
		return opExtInst(_pTypeInstr, ExtName, extinstr::GLSLstd450::GLSLstd450MatrixInverse, _pMatrix);
	}

	getModule()->logError("Operand of opMatrixInverse is not a square matirx");

	return nullptr;
}