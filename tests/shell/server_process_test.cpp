/**
 *
 *  @file server_process_test.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2025, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/vix
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Vix.cpp
 *
 */
#include <cassert>
#include <iostream>
#include <string>

#include <vix/ui/shell/ServerProcess.hpp>
#include <vix/ui/support/Result.hpp>

using namespace vix::ui;

static void test_server_process_default_state()
{
  ServerProcess process;

  assert(process.command().empty());
  assert(process.working_directory().empty());
  assert(!process.has_command());
  assert(!process.has_working_directory());
  assert(process.stopped());
  assert(!process.running());
  assert(process.pid() == -1);
}

static void test_server_process_setters()
{
  ServerProcess process;

  process.set_command("sleep 30");
  process.set_working_directory("/tmp");

  assert(process.command() == "sleep 30");
  assert(process.working_directory() == "/tmp");
  assert(process.has_command());
  assert(process.has_working_directory());
}

static void test_server_process_construct_with_command()
{
  ServerProcess process("sleep 30");

  assert(process.command() == "sleep 30");
  assert(process.working_directory().empty());
  assert(process.has_command());
  assert(!process.has_working_directory());
}

static void test_server_process_construct_with_command_and_directory()
{
  ServerProcess process("sleep 30", "/tmp");

  assert(process.command() == "sleep 30");
  assert(process.working_directory() == "/tmp");
  assert(process.has_command());
  assert(process.has_working_directory());
}

static void test_server_process_start_without_command_fails()
{
  ServerProcess process;

  Result<void> result = process.start();

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ConfigError);
  assert(!result.error_message().empty());
  assert(process.stopped());
}

static void test_server_process_start_and_stop()
{
#if defined(__unix__) || defined(__APPLE__)
  ServerProcess process("sleep 30");

  Result<void> start_result = process.start();

  assert(start_result.is_ok());
  assert(process.running());
  assert(!process.stopped());
  assert(process.pid() > 0);

  Result<void> stop_result = process.stop();

  assert(stop_result.is_ok());
  assert(process.stopped());
  assert(!process.running());
  assert(process.pid() == -1);
#else
  ServerProcess process("sleep 30");

  Result<void> start_result = process.start();

  assert(start_result.is_failed());
  assert(process.stopped());
#endif
}

static void test_server_process_start_is_idempotent()
{
#if defined(__unix__) || defined(__APPLE__)
  ServerProcess process("sleep 30");

  Result<void> first = process.start();
  Result<void> second = process.start();

  assert(first.is_ok());
  assert(second.is_ok());
  assert(process.running());
  assert(process.pid() > 0);

  Result<void> stop_result = process.stop();

  assert(stop_result.is_ok());
  assert(process.stopped());
#else
  ServerProcess process("sleep 30");

  Result<void> result = process.start();

  assert(result.is_failed());
#endif
}

static void test_server_process_stop_is_idempotent()
{
  ServerProcess process("sleep 30");

  Result<void> first = process.stop();
  Result<void> second = process.stop();

  assert(first.is_ok());
  assert(second.is_ok());
  assert(process.stopped());
  assert(process.pid() == -1);
}

static void test_server_process_move_constructor()
{
#if defined(__unix__) || defined(__APPLE__)
  ServerProcess process("sleep 30");

  Result<void> start_result = process.start();

  assert(start_result.is_ok());
  assert(process.running());

  const std::int64_t original_pid = process.pid();

  ServerProcess moved(std::move(process));

  assert(moved.running());
  assert(moved.pid() == original_pid);

  assert(process.stopped());
  assert(process.pid() == -1);

  Result<void> stop_result = moved.stop();

  assert(stop_result.is_ok());
  assert(moved.stopped());
#else
  ServerProcess process("sleep 30");
  ServerProcess moved(std::move(process));

  assert(moved.command() == "sleep 30");
  assert(moved.stopped());
#endif
}

static void test_server_process_move_assignment()
{
#if defined(__unix__) || defined(__APPLE__)
  ServerProcess source("sleep 30");
  ServerProcess target;

  Result<void> start_result = source.start();

  assert(start_result.is_ok());
  assert(source.running());

  const std::int64_t original_pid = source.pid();

  target = std::move(source);

  assert(target.running());
  assert(target.pid() == original_pid);

  assert(source.stopped());
  assert(source.pid() == -1);

  Result<void> stop_result = target.stop();

  assert(stop_result.is_ok());
  assert(target.stopped());
#else
  ServerProcess source("sleep 30");
  ServerProcess target;

  target = std::move(source);

  assert(target.command() == "sleep 30");
  assert(target.stopped());
#endif
}

static void test_server_process_invalid_working_directory_fails_child_only()
{
#if defined(__unix__) || defined(__APPLE__)
  ServerProcess process("sleep 30", "/path/that/does/not/exist/vix-ui-test");

  Result<void> start_result = process.start();

  /*
   * The parent can only know that fork() succeeded. The child exits with
   * 127 if chdir() fails. This test ensures the parent-side lifecycle still
   * remains safe and stoppable.
   */
  assert(start_result.is_ok());
  assert(process.running());

  Result<void> stop_result = process.stop();

  assert(stop_result.is_ok());
  assert(process.stopped());
#else
  ServerProcess process("sleep 30", "/path/that/does/not/exist/vix-ui-test");

  Result<void> start_result = process.start();

  assert(start_result.is_failed());
#endif
}

int main()
{
  test_server_process_default_state();
  test_server_process_setters();
  test_server_process_construct_with_command();
  test_server_process_construct_with_command_and_directory();
  test_server_process_start_without_command_fails();
  test_server_process_start_and_stop();
  test_server_process_start_is_idempotent();
  test_server_process_stop_is_idempotent();
  test_server_process_move_constructor();
  test_server_process_move_assignment();
  test_server_process_invalid_working_directory_fails_child_only();

  std::cout << "[OK] ui server process tests passed\n";
  return 0;
}
