#include "CLI11.hpp"
#include "engine_config.h"

int main(int argc, char **argv) {
  CLI::App app{"Image Engine"};
  AppContext ctx;

  // Configure the CLI
  setup_cli(app, ctx);

  // Parse (CLI11 handles errors automatically)
  CLI11_PARSE(app, argc, argv);

  // Run the logic
  run_operations(ctx);

  return 0;
}