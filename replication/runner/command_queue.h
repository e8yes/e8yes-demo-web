/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */

#ifndef REPLICATION_COMMAND_QUEUE_H
#define REPLICATION_COMMAND_QUEUE_H

#include <string>

#include "proto_cc/command.pb.h"
#include "replication/raft/journal.h"
#include "replication/runner/fulfillment_pool.h"

namespace e8 {

/**
 * @brief The CommandInterface class Outlines how the actual command runner will receive a command
 * as well as how it should return the result.
 */
class CommandRunnerInterface {
  public:
    CommandRunnerInterface();
    virtual ~CommandRunnerInterface();

    /**
     * @brief Run Runs the specified command given the class's current state and returns the result
     * from the run.
     */
    virtual std::string Run(std::string const &command) = 0;

    /**
     * @brief Reset When the replication node gets shutdown in a integration testing situation, this
     * function gets called to undo all the applied commands.
     */
    virtual void Reset();
};

/**
 * @brief The CommandQueueProcessor class Processes committed commands from Raft.
 */
class CommandQueueProcessor : public RaftCommitListener {
  public:
    /**
     * @brief CommandQueueProcessor Constructs a processor that takes unique committed commands to
     * the custom command runner and stores the result to the fulfillment pool.
     *
     * @param fulfillments The fulfillment pool to store the command run result.
     * @param runner A custom command runner defining the actual computation and produces the result
     * of the computation.
     */
    CommandQueueProcessor(FulfillmentPool *fulfillments, CommandRunnerInterface *runner);
    ~CommandQueueProcessor() override;

    void Apply(CommandEntry const &entry) override;

  private:
    FulfillmentPool *fulfillments_;
    CommandRunnerInterface *runner_;
};

} // namespace e8

#endif // REPLICATION_COMMAND_QUEUE_H
