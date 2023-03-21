#ifndef LLVM_INFERENCE_ENGINE_DRIVER_H
#define LLVM_INFERENCE_ENGINE_DRIVER_H

#include "agent.h"
#include "environment.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
#include <map>
#include <string>


class InferenceEngine {
  Environment *env;

public:
  llvm::StringMap<Agent *> agents;

  void addAgent(Agent *agent, std::string name) {
    if (agents.find(name) == agents.end()) {
      agents[name] = agent;
    } else {
      // throw error
      LLVM_DEBUG(llvm::errs() << "ERROR: Agent with the name " << name
                << " already exists. Please give a different name!\n");
    }
  }

  void setEnvironment(Environment *_env) {
    assert(_env && "Invalid Environment!");
    env = _env;
  }

  Environment *getEnvironment() { return env; }

  // virtual void getInfo() = 0;

  void computeAction() {
    while (true) {
      Action action;

      // current agent
      auto current_agent = this->agents[this->env->getNextAgent()];

      auto obs = this->env->getCurrentObservation(this->env->getNextAgent());
      LLVM_DEBUG(llvm::outs() << "Before Next agent in compute action is: " << this->env->getNextAgent()
             << " with obs size: " << obs.size() << "\n");
      action = current_agent->computeAction(obs);
      this->env->step(action);
      LLVM_DEBUG(llvm::outs() << "After Next agent in compute action is: " << this->env->getNextAgent()
             << " with obs size: " << obs.size() << "\n");
      if (this->env->checkDone()) {
        LLVM_DEBUG(llvm::outs() << "Done🎉\n");
        break;
      }
    }
  }
};

#endif