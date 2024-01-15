//=== SerDes/tensorflowSerDes.h - SerDes for TF support ---*- C++ ---------===//
//
// Part of the MLCompilerBridge Project, under the Apache 2.0 License.
// See the LICENSE file under home directory for license and copyright
// information.
//
//===----------------------------------------------------------------------===//
//
// Serialization/Deserialization to support TF AOT models.
//
//===----------------------------------------------------------------------===//

#ifndef TENSORFLOW_SERIALIZER_H
#define TENSORFLOW_SERIALIZER_H

#include "SerDes/baseSerDes.h"
#include "tensorflow/compiler/tf2xla/xla_compiled_cpu_function.h"

namespace MLBridge {
class TensorflowSerDes : public BaseSerDes {
public:
  TensorflowSerDes() : BaseSerDes(Kind::Tensorflow) {}

  static bool classof(const BaseSerDes *S) {
    return S->getKind() == BaseSerDes::Kind::Tensorflow;
  }

#define SET_FEATURE(TYPE)                                                      \
  void setFeature(const std::string &, const TYPE &) override;                 \
  void setFeature(const std::string &, const std::vector<TYPE> &) override;
  SUPPORTED_TYPES(SET_FEATURE)
#undef SET_FEATURE

  void setRequest(void *request) override {
    CompiledModel =
        reinterpret_cast<tensorflow::XlaCompiledCpuFunction *>(request);
  }

  void *getSerializedData() override{};
  void cleanDataStructures() override{};

private:
  void *deserializeUntyped(void *data) override{};
  tensorflow::XlaCompiledCpuFunction *CompiledModel;
};
} // namespace MLBridge

#endif
