#pragma once

#include <exception>
#include <vector>
#include <list>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <sstream>

// Undirected Weighted Simple Graph
class UndirectedSimpleGraph
{
public :
  UndirectedSimpleGraph();

  bool addVertex(const std::string& name);

  bool removeVertex(const std::string& name);

  bool addEdge(
    const std::string& vertexId1,
    const std::string& vertexId2,
    const std::string& name,
    const int& weight  = 1
  );

  bool containsVertex(const std::string& name) const;
  bool containsEdge(const std::string& name) const;

  bool removeEdge(const std::string & edgeId);
  bool areAdjacent(
    const std::string& vertexId1,
    const std::string&vertexId2
  ) const;

  std::unordered_set<std::string> getIncidentEdges(
    const std::string& vertexId
  ) const;

  int getVertexDegree(const std::string& id) const;
  int getVertexNumber() const;
  int getEdgeNumber() const;

  // Kruskal
  UndirectedSimpleGraph getKruskalMinimumSpanningTree();

  // Prims
  UndirectedSimpleGraph getPrimsMinimumSpanningTree(const std::string& start);

  // Dijkstra
  UndirectedSimpleGraph getDijkstraShortestPaths(const std::string& start);

  friend std::ostream& operator << (
    std::ostream& os,
    const UndirectedSimpleGraph& g
  );

private :
  struct EdgeInfo
  {
    std::string m_Vertex1;
    std::string m_Vertex2;
    int m_Weight;
  };

  std::unordered_map<std::string, EdgeInfo> m_Edges;

  std::unordered_map<std::string, std::unordered_set<std::string>> m_Vertices;
};