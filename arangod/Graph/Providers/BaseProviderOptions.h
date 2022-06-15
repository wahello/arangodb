////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2022 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Heiko Kernbach
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Aql/Expression.h"
#include "Aql/FixedVarExpressionContext.h"
#include "Aql/NonConstExpressionContainer.h"
#include "Aql/Projections.h"
#include "Cluster/ClusterInfo.h"
#include "Graph/Cache/RefactoredClusterTraverserCache.h"
#include "Transaction/Methods.h"
#include "Aql/InAndOutRowExpressionContext.h"

#ifdef USE_ENTERPRISE
#include "Enterprise/Graph/Providers/SmartGraphRPCCommunicator.h"
#endif

#include <optional>
#include <vector>

namespace arangodb {

namespace aql {
class QueryContext;
}

namespace graph {

struct IndexAccessor {
  IndexAccessor(transaction::Methods::IndexHandle idx, aql::AstNode* condition,
                std::optional<size_t> memberToUpdate,
                std::unique_ptr<arangodb::aql::Expression> expression,
                std::optional<aql::NonConstExpressionContainer> nonConstPart,
                size_t cursorId, TRI_edge_direction_e direction);
  IndexAccessor(IndexAccessor const&) = delete;
  IndexAccessor(IndexAccessor&&) = default;
  IndexAccessor& operator=(IndexAccessor const&) = delete;

  aql::AstNode* getCondition() const noexcept;
  aql::Expression* getExpression() const noexcept;
  transaction::Methods::IndexHandle indexHandle() const noexcept;
  std::optional<size_t> getMemberToUpdate() const;
  size_t cursorId() const noexcept;
  TRI_edge_direction_e direction() const noexcept;

  bool hasNonConstParts() const noexcept;

  aql::NonConstExpressionContainer const& nonConstPart() const;

 private:
  transaction::Methods::IndexHandle _idx;
  aql::AstNode* _indexCondition;
  // Position of _from / _to in the index search condition
  std::optional<size_t> _memberToUpdate;
  std::unique_ptr<arangodb::aql::Expression> _expression;
  size_t _cursorId;
  std::optional<aql::NonConstExpressionContainer> _nonConstContainer;
  TRI_edge_direction_e const _direction;
};

struct SingleServerBaseProviderOptions {
  using WeightCallback = std::function<double(
      double originalWeight, arangodb::velocypack::Slice edge)>;

 public:
  SingleServerBaseProviderOptions(
      aql::Variable const* tmpVar,
      std::pair<std::vector<IndexAccessor>,
                std::unordered_map<uint64_t, std::vector<IndexAccessor>>>&&
          indexInfo,
      aql::FixedVarExpressionContext& expressionContext,
      std::vector<std::pair<aql::Variable const*, aql::RegisterId>>
          filterConditionVariables,
      std::unordered_map<std::string, std::vector<std::string>> const&
          collectionToShardMap,
      aql::Projections const& vertexProjections,
      aql::Projections const& edgeProjections);

  SingleServerBaseProviderOptions(SingleServerBaseProviderOptions const&) =
      delete;
  SingleServerBaseProviderOptions(SingleServerBaseProviderOptions&&) = default;

  aql::Variable const* tmpVar() const;
  std::pair<std::vector<IndexAccessor>,
            std::unordered_map<uint64_t, std::vector<IndexAccessor>>>&
  indexInformations();

  std::unordered_map<std::string, std::vector<std::string>> const&
  collectionToShardMap() const;

  aql::FixedVarExpressionContext& expressionContext() const;

  void prepareContext(aql::InputAqlItemRow input);
  void unPrepareContext();

  bool hasWeightMethod() const;

  double weightEdge(double prefixWeight,
                    arangodb::velocypack::Slice edge) const;

  void setWeightEdgeCallback(WeightCallback callback);

  aql::Projections const& getVertexProjections() const;

  aql::Projections const& getEdgeProjections() const;

 private:
  // The temporary Variable used in the Indexes
  aql::Variable const* _temporaryVariable;
  // One entry per collection, ShardTranslation needs
  // to be done by Provider
  std::pair<std::vector<IndexAccessor>,
            std::unordered_map<uint64_t, std::vector<IndexAccessor>>>
      _indexInformation;

  // The context of AQL variables. These variables are set from the outside.
  // and the caller needs to make sure the reference stays valid
  aql::FixedVarExpressionContext& _expressionContext;

  // CollectionName to ShardMap, used if the Traversal is pushed down to
  // DBServer
  // Ownership of this _collectionToShardMap stays at the BaseOptions, and is
  // not transferred into this class.
  std::unordered_map<std::string, std::vector<std::string>> const&
      _collectionToShardMap;

  // Optional callback to compute the weight of an edge.
  std::optional<WeightCallback> _weightCallback;

  // TODO: Currently this will be a copy. As soon as we remove the old
  // non-refactored code, we will do a move instead of a copy operation.
  std::vector<std::pair<aql::Variable const*, aql::RegisterId>>
      _filterConditionVariables;

  /// @brief Projections used on vertex data
  /// Ownership of this struct is at the BaseOptions
  aql::Projections const& _vertexProjections;

  /// @brief Projections used on edge data.
  /// Ownership of this struct is at the BaseOptions
  aql::Projections const& _edgeProjections;
};

struct ClusterBaseProviderOptions {
  using WeightCallback = std::function<double(
      double originalWeight, arangodb::velocypack::Slice edge)>;

 public:
  ClusterBaseProviderOptions(
      std::shared_ptr<RefactoredClusterTraverserCache> cache,
      std::unordered_map<ServerID, aql::EngineId> const* engines, bool backward,
      bool produceVertices);

  ClusterBaseProviderOptions(
      std::shared_ptr<RefactoredClusterTraverserCache> cache,
      std::unordered_map<ServerID, aql::EngineId> const* engines, bool backward,
      bool produceVertices, aql::FixedVarExpressionContext* expressionContext,
      std::vector<std::pair<aql::Variable const*, aql::RegisterId>>
          filterConditionVariables,
      std::unordered_set<uint64_t> availableDepthsSpecificConditions);

  RefactoredClusterTraverserCache* getCache();

  bool isBackward() const;

  bool produceVertices() const;

  [[nodiscard]] std::unordered_map<ServerID, aql::EngineId> const* engines()
      const;

  // [GraphRefactor] Note: Both used in SingleServer and Cluster variant.
  // If more overlaps reoccur, we might want to implement a base class.
  void prepareContext(aql::InputAqlItemRow input);
  void unPrepareContext();
  aql::FixedVarExpressionContext* expressionContext();

  bool hasWeightMethod() const;

  double weightEdge(double prefixWeight,
                    arangodb::velocypack::Slice edge) const;

  void setWeightEdgeCallback(WeightCallback callback);

#ifdef USE_ENTERPRISE
  void setRPCCommunicator(
      std::unique_ptr<enterprise::SmartGraphRPCCommunicator>);
  enterprise::SmartGraphRPCCommunicator& getRPCCommunicator();
#endif

  bool hasDepthSpecificLookup(uint64_t depth) const noexcept;

 private:
  std::shared_ptr<RefactoredClusterTraverserCache> _cache;

  std::unordered_map<ServerID, aql::EngineId> const* _engines;

  bool _backward;

  bool _produceVertices;

  // [GraphRefactor] Note: All vars below used in SingleServer && Cluster case
  aql::FixedVarExpressionContext* _expressionContext;

  // TODO: Currently this will be a copy. As soon as we remove the old
  // non-refactored code, we will do a move instead of a copy operation.
  std::vector<std::pair<aql::Variable const*, aql::RegisterId>>
      _filterConditionVariables;

  // Optional callback to compute the weight of an edge.
  std::optional<WeightCallback> _weightCallback;

#ifdef USE_ENTERPRISE
  // TODO: This is right now a little bit hacked in, to allow unittestability.
  // i would like to clean this up better such that the RPCCommunicator is
  // properly owned by this class
  // Ticket [GORDO-1392]
  std::unique_ptr<enterprise::SmartGraphRPCCommunicator> _communicator;
#endif

  std::unordered_set<uint64_t> _availableDepthsSpecificConditions;
};

}  // namespace graph
}  // namespace arangodb
