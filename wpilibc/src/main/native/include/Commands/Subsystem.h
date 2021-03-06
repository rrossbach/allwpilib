/*----------------------------------------------------------------------------*/
/* Copyright (c) 2011-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <memory>

#include <wpi/StringRef.h>
#include <wpi/Twine.h>

#include "ErrorBase.h"
#include "SmartDashboard/Sendable.h"
#include "SmartDashboard/SendableBase.h"

namespace frc {

class Command;

class Subsystem : public ErrorBase, public SendableBase {
  friend class Scheduler;

 public:
  explicit Subsystem(const wpi::Twine& name);

  void SetDefaultCommand(Command* command);
  Command* GetDefaultCommand();
  wpi::StringRef GetDefaultCommandName();
  void SetCurrentCommand(Command* command);
  Command* GetCurrentCommand() const;
  wpi::StringRef GetCurrentCommandName() const;
  virtual void Periodic();
  virtual void InitDefaultCommand();

  void AddChild(const wpi::Twine& name, std::shared_ptr<Sendable> child);
  void AddChild(const wpi::Twine& name, Sendable* child);
  void AddChild(const wpi::Twine& name, Sendable& child);
  void AddChild(std::shared_ptr<Sendable> child);
  void AddChild(Sendable* child);
  void AddChild(Sendable& child);

 private:
  void ConfirmCommand();

  Command* m_currentCommand = nullptr;
  bool m_currentCommandChanged = true;
  Command* m_defaultCommand = nullptr;
  bool m_initializedDefaultCommand = false;

 public:
  void InitSendable(SendableBuilder& builder) override;
};

}  // namespace frc
