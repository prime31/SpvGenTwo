#include "example/GeometryShader.h"
#include "spvgentwo/TypeAlias.h"

using namespace spvgentwo;

Module examples::geometryShader(IAllocator* _pAllocator, ILogger* _pLogger)
{
    using namespace glsl;

    // create a new spir-v module
    Module module(_pAllocator, spv::Version, spv::AddressingModel::Logical, spv::MemoryModel::GLSL450, _pLogger);

    // configure capabilities and extensions
    module.addCapability(spv::Capability::Geometry);
    module.addExtension("GLSL.std.450"); // needed?
    module.addSourceStringInstr()->opSource(spv::SourceLanguage::GLSL, 330u);

    EntryPoint& entry = module.addEntryPoint(spv::ExecutionModel::Geometry, "main");
    entry.addExecutionMode(spv::ExecutionMode::InputPoints);
    entry.addExecutionMode(spv::ExecutionMode::Invocations, 1u);
    entry.addExecutionMode(spv::ExecutionMode::OutputLineStrip);
    entry.addExecutionMode(spv::ExecutionMode::OutputVertices, 2u);

    // global variables
    Instruction* uniOffset = module.uniform<vec3>("u_offset");

    Instruction* inPos = module.input<vec3>("g_inPosition");
    Instruction* inNormal = module.input<vec3>("g_inNormal");

    Instruction* outPerVertex = module.output<vec3>("g_outPosition");

    // vec3 computePos()
    Function& computePos = module.addFunction<vec3>("computePos", spv::FunctionControlMask::Const);
    {
        BasicBlock& bb = *computePos; // get entry block to this function

        Instruction* normal = bb->opLoad(inNormal);
        Instruction* pos = bb->opLoad(inPos);
        Instruction* offset = bb->opLoad(uniOffset);

        Instruction* out = bb->Mul(normal, offset);
        out = bb->Add(out, pos);

        bb.returnValue(out);
    }

    // void main();
    {


        BasicBlock& bb = *entry; // get entry block to this function       

        bb->opEmitVertex();

        entry->opReturn();
    }

    return module;
}
