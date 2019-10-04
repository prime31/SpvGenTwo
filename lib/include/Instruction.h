#pragma once

#include "List.h"
#include "Operand.h"
#include "Flag.h"

namespace spvgentwo
{
	// forward delcs
	class Type;
	class Function;
	class Module;

	class Instruction : public List<Operand>
	{
	public:
		using Iterator = EntryIterator<Operand>;

		template <class ...Args>
		Instruction(Module* _pModule, const spv::Op _op = spv::Op::OpNop, Args&& ... _args);
		template <class ...Args>
		Instruction(BasicBlock* _pBasicBlock, const spv::Op _op = spv::Op::OpNop, Args&& ... _args);

		~Instruction();

		Module* getModule();
		const Module* getModule() const;

		BasicBlock* getBasicBlock() { return m_pBasicBlock; }
		const BasicBlock* getBasicBlock() const { return m_pBasicBlock; }

		// manual instruction construction:
		void setOperation(const spv::Op _op) { m_Operation = _op; };
		spv::Op getOperation() const { return m_Operation; }
		template<class ...Args>
		Operand& addOperand(Args&& ... _operand) { return emplace_back(stdrep::forward<Args>(_operand)...); }

		spv::Id getResultId() const;
		Instruction* getType() const;

		// get StorageClass of OpVariable instructions
		spv::StorageClass getStorageClass() const;
	
		bool isTypeOp() const;

		bool hasResult() const { return hasResultId(m_Operation); }
		bool hasResultType() const { return hasResultTypeId(m_Operation); }
		bool hasResultAndType() const { return hasResultAndTypeId(m_Operation); }

		void reset();

		// get number of 32 bit words used by this instruction
		unsigned int getWordCount() const;
		unsigned int getOpCode() const;

		void write(IWriter* _pWriter, spv::Id& _resultId);

		template <class ...Args>
		Instruction* makeOp(const spv::Op _op, Args ... _args);

		template <class ...Args>
		void appendLiterals(Args ... _args);

		void opNop();

		Instruction* opUndef(Instruction* _pResultType);

		Instruction* opSizeOf(Instruction* _pResultType, Instruction* _pPointerToVar);

		// instruction generators:
		// all instructions generating a result id return a pointer to this instruction for reference (passing to other instruction operand)
		void opCapability(const spv::Capability _capability);

		void opMemoryModel(const spv::AddressingModel _addressModel, const spv::MemoryModel _memoryModel);

		void opExtension(const char* _pExtName);

		// generates extension id
		Instruction* opExtInstImport(const char* _pExtName);

		template <class ...Operands>
		Instruction* opExtInst(Instruction* _pResultType, Instruction* _pExtensionId, unsigned int _instOpCode, Operands ... _operands);

		Instruction* opLabel();

		Instruction* opFunction(const Flag<spv::FunctionControlMask> _functionControl, Instruction* _pResultType, Instruction* _pFuncType);

		Instruction* opFunctionParameter(Instruction* _pType);

		void opReturn();

		void opReturnValue(Instruction* _pValue);

		void opFunctionEnd();

		template <class ... ArgInstr>
		Instruction* opFunctionCall(Instruction* _pResultType, Instruction* _pFunction, ArgInstr ... _args);

		template <class ... ArgInstr>
		Instruction* call(Function* _pFunction, ArgInstr ... _args);

		//  _pFunction is result of opFunction
		template <class ... Instr>
		void opEntryPoint(const spv::ExecutionModel _model, Instruction* _pFunction, const char* _pName, Instr ... _instr);

		// _pResultType must be of OpTypePointer
		template <class ...Instr>
		Instruction* opVariable(Instruction* _pResultType, const spv::StorageClass _storageClass, Instr ... _initializer);

		void opName(Instruction* _pTarget, const char* _pName);

		void opMemberName(Instruction* _pTargetStructType, unsigned int _memberIndex, const char* _pName);

		template <class ... Decorations>
		void opDecorate(Instruction* _pTarget, spv::Decoration _decoration, Decorations ... _decorations);

		template <class ... Decorations>
		void opMemberDecorate(Instruction* _pTargetStructType, unsigned int _memberIndex, spv::Decoration _decoration, Decorations ... _decorations);

		template <class ... Instr>
		void opMemberId(Instruction* _pTarget, spv::Decoration _decoration, Instruction* _pId, Instr*..._ids);

		template <class ... VarParents>
		Instruction* opPhi(Instruction* _pResultType, Instruction* _pVar, BasicBlock* _pVarBB, VarParents ... _parents);

		// deduce parent form input variables
		template <class ... VarInst>
		Instruction* opPhiEx(Instruction* _pVar, VarInst* ... _variables);
	
		template <class ...LoopControlParams>
		void opLoopMerge(Instruction* _pMergeLabel, Instruction* _pContinueLabel, const Flag<spv::LoopControlMask> _loopControl, LoopControlParams ... _params);

		template <class ...LoopControlParams>
		void opLoopMergeEx(BasicBlock* _pMergeBlock, BasicBlock* _pContinueBlock, const Flag<spv::LoopControlMask> _loopControl, LoopControlParams ... _params);

		void opSelectionMerge(Instruction* _pMergeLabel, const spv::SelectionControlMask _control);
		void opSelectionMergeEx(BasicBlock* _pMergeBlock, const spv::SelectionControlMask _control);

		void opBranch(Instruction* _pTargetLabel);

		// label is infered from the basic block on serialization
		void opBranchEx(BasicBlock* _pTargetBlock);

		void opBranchConditional(Instruction* _pCondition, Instruction* _pTrueLabel, Instruction* _pFalseLabel);
		void opBranchConditional(Instruction* _pCondition, Instruction* _pTrueLabel, Instruction* _pFalseLabel, const unsigned int _trueWeight, const unsigned int _falseWeight);

		// label is infered from the basic block on serialization
		void opBranchConditionalEx(Instruction* _pCondition, BasicBlock* _pTrueBlock, BasicBlock* _pFalseBlock);
		void opBranchConditionalEx(Instruction* _pCondition, BasicBlock* _pTrueBlock, BasicBlock* _pFalseBlock, const unsigned int _trueWeight, const unsigned int _falseWeight);

		Instruction* opIAdd(Instruction* _pResultType, Instruction* _pLeft, Instruction* _pRight);
		Instruction* opIAddEx(Instruction* _pLeft, Instruction* _pRight);

		Instruction* opISub(Instruction* _pResultType, Instruction* _pLeft, Instruction* _pRight);
		Instruction* opISubEx(Instruction* _pLeft, Instruction* _pRight);

		Instruction* opIMul(Instruction* _pResultType, Instruction* _pLeft, Instruction* _pRight);
		Instruction* opIMulEx(Instruction* _pLeft, Instruction* _pRight);

	private:
		void resolveId(spv::Id& _resultId);

		// creates literals
		template <class T, class ...Args>
		void makeOpInternal(T first, Args ... _args);

		template <class ... VarInst>
		Instruction* opPhiExInternal(Instruction* _pVar, VarInst* ... _variables);

	private:
		spv::Op m_Operation = spv::Op::OpNop;
		BasicBlock* m_pBasicBlock = nullptr; // parent
		Module* m_pModule = nullptr;
	};

	// free helper function
	void writeInstructions(IWriter* _pWriter, const List<Instruction>& _instructions, spv::Id& _resultId);

	template<class ...Args>
	inline Instruction::Instruction(Module* _pModule, const spv::Op _op, Args&& ..._args) :
		m_pBasicBlock(nullptr), m_pModule(_pModule), List(_pModule->getAllocator())
	{
		makeOp(_op, stdrep::forward<Args>(_args)...);
	}

	template<class ...Args>
	inline Instruction::Instruction(BasicBlock* _pBasicBlock, const spv::Op _op, Args&& ..._args) :
		m_pBasicBlock(_pBasicBlock), m_pModule(_pBasicBlock->getModule()), List(_pBasicBlock->getAllocator())
	{
		makeOp(_op, stdrep::forward<Args>(_args)...);
	}

	template<class ...Args>
	inline Instruction* Instruction::makeOp(const spv::Op _op, Args ..._args)
	{
		reset();

		m_Operation = _op;

		if constexpr (sizeof...(_args) > 0u)
		{
			makeOpInternal(_args...);
		}

		return this;
	}
	
	template<class T, class ...Args>
	inline void Instruction::makeOpInternal(T _first, Args ..._args)
	{
		if constexpr (is_same_base_type_v<T, Instruction*> || is_same_base_type_v<T, BasicBlock*> || is_same_base_type_v<T, spv::Id> || is_same_base_type_v<T, literal_t>)
		{
			addOperand(_first);
		}
		else if constexpr (sizeof(T) == sizeof(literal_t)) // bitcast to 32 bit literal
		{
			addOperand(*reinterpret_cast<const literal_t*>(&_first));
		}
		else
		{
			appendLiterals(_first);
		}

		if constexpr (sizeof...(_args) > 0u)
		{
			makeOpInternal(_args...);
		}
	}

	template<class ...Args>
	inline void Instruction::appendLiterals(Args ..._args)
	{
		appendLiteralsToContainer(*this, _args...);
	}

	template<class ...Operands>
	inline Instruction* Instruction::opExtInst(Instruction* _pResultType, Instruction* _pExtensionId, unsigned int _instOpCode, Operands ..._operands)
	{
		return makeOp(spv::Op::OpExtInst, _pResultType, InvalidId, _pExtensionId, literal_t{ _instOpCode }, _operands...);
	}

	template<class ...ArgInstr>
	inline Instruction* Instruction::opFunctionCall(Instruction* _pResultType, Instruction* _pFunction, ArgInstr ..._args)
	{
		return makeOp(spv::Op::OpFunctionCall, _pResultType, InvalidId, _pFunction, _args...);
	}

	template<class ...ArgInstr>
	inline Instruction* Instruction::call(Function* _pFunction, ArgInstr ..._args)
	{
		return opFunctionCall(_pFunction->getReturnType(), _pFunction->getFunction(), _args...);
	}

	template<class ...Instr>
	inline void Instruction::opEntryPoint(const spv::ExecutionModel _model, Instruction* _pFunction, const char* _pName, Instr ..._instr)
	{
		makeOp(spv::Op::OpEntryPoint, _model, _pFunction, _pName, _instr...);
	}

	template<class ...Instr>
	inline Instruction* Instruction::opVariable(Instruction* _pResultType, const spv::StorageClass _storageClass, Instr ..._initializer)
	{
		return makeOp(spv::Op::OpVariable, _pResultType, InvalidId, _initializer....);
	}

	template<class ...Decorations>
	inline void Instruction::opDecorate(Instruction* _pTarget, spv::Decoration _decoration, Decorations ..._decorations)
	{
		makeOp(spv::Op::OpDecorate, _pTarget, _decoration, _decorations...);
	}

	template<class ...Decorations>
	inline void Instruction::opMemberDecorate(Instruction* _pTargetStructType, unsigned int _memberIndex, spv::Decoration _decoration, Decorations ..._decorations)
	{
		makeOp(spv::Op::OpMemberDecorate, _pTargetStructType, _memberIndex, _decoration, _decorations...);
	}

	template<class ...Instr>
	inline void Instruction::opMemberId(Instruction* _pTarget, spv::Decoration _decoration, Instruction* _pId, Instr* ..._ids)
	{
		makeOp(spv::Op::OpDecorateId, _pTarget, _decoration, _ids...);
	}

	template<class ...VarParents>
	inline Instruction* Instruction::opPhi(Instruction* _pResultType, Instruction* _pVar, BasicBlock* _pVarBB, VarParents ..._parents)
	{
		return makeOp(spv::Op::OpPhi, _pResultType, InvalidId, _pVar, _pVarBB, _parents...);
	}

	template<class ...VarInst>
	inline Instruction* Instruction::opPhiEx(Instruction* _pVar, VarInst* ..._variables)
	{
		makeOp(spv::Op::OpPhi, _pVar->getType(), InvalidId);
		return opPhiExInternal(_pVar, _variables...);
	}

	template<class ...VarInst>
	inline Instruction* Instruction::opPhiExInternal(Instruction* _pVar, VarInst* ..._variables)
	{
		addOperand(_pVar);
		addOperand(_pVar->getBasicBlock());

		if constexpr(sizeof...(_variables) > 0)
		{
			opPhiExInternal(_variables...);
		}

		return this;
	}

	template<class ...LoopControlParams>
	inline void Instruction::opLoopMerge(Instruction* _pMergeLabel, Instruction* _pContinueLabel, const Flag<spv::LoopControlMask> _loopControl, LoopControlParams ..._params)
	{
		makeOp(spv::Op::OpLoopMerge, _pMergeLabel, _pContinueLabel, literal_t{ _loopControl }, _params...);
	}

	template<class ...LoopControlParams>
	inline void Instruction::opLoopMergeEx(BasicBlock* _pMergeBlock, BasicBlock* _pContinueBlock, const Flag<spv::LoopControlMask> _loopControl, LoopControlParams ..._params)
	{
		makeOp(spv::Op::OpLoopMerge, _pMergeBlock, _pContinueBlock, literal_t{ _loopControl }, _params...);
	}
} // !spvgentwo