#ifndef GLOW_IR_INSTRS_H
#define GLOW_IR_INSTRS_H

#include "glow/IR/IR.h"
#include "glow/IR/Type.h"

namespace glow {

class AllocInst : public Instruction {
  TypeRef Ty_;

public:
  AllocInst(TypeRef T) : Instruction(), Ty_(T) {}
  StringRef getValueName() override { return "Alloc"; }
  std::string getExtraDesc() override { return Ty_->asString(); }
  TypeRef getType() { return Ty_; }
};

class DeallocInst : public Instruction {
public:
  DeallocInst(AllocInst *A) : Instruction({{A, OperandKind::kIn}}) {}
  StringRef getValueName() override { return "Dealloc"; }
};

class CopyInst : public Instruction {
public:
  CopyInst(Value *dest, Value *src)
      : Instruction({{dest, OperandKind::kOut}, {src, OperandKind::kIn}}) {}
  StringRef getValueName() override { return "Copy"; }
};

class ReturnInst : public Instruction {
public:
  ReturnInst(Value *src) : Instruction({{src, OperandKind::kIn}}) {}
  StringRef getValueName() override { return "Return"; }
};

class ReluInst : public Instruction {
public:
  ReluInst(Value *dest, Value *src)
      : Instruction({{dest, OperandKind::kOut}, {src, OperandKind::kIn}}) {}
  StringRef getValueName() override { return "Relu"; }
};

class StaticVariable : public Value {
public:
  enum class InitKind {
    kExtern,    // No initialization.
    kBroadcast, // Broadcast a single value to all elements.
    kXavier,    // Init the tensor with random values using the Xavier method.
  };

  const char *getKindStr(InitKind K) {
    const char *names[] = {"extern", "broadcast", "xavier", nullptr};
    return names[(int)K];
  }

private:
  // The type of the tensor to allocate.
  TypeRef Ty_;

  /// The value to use during initialization. This can be the value to splat or
  /// a parameter to specify the range of the random values.
  float val_;

  /// The initialization mode.
  InitKind mode_;

public:
  StaticVariable(TypeRef Ty, InitKind mode, float val)
      : Value(), Ty_(Ty), val_(val), mode_(mode) {}
  StringRef getValueName() override { return "Static"; }
  InitKind getMode() { return mode_; }
  float getVal() { return val_; }
  TypeRef getType() { return Ty_; }

  std::string getExtraDesc() override {
    return ", " + Ty_->asString() + " " + std::to_string(val_) + ", " +
           getKindStr(mode_);
  }
};

} // namespace glow

#endif // GLOW_IR_INSTRS_H
