#include "undirectedSimpleGraph.h"

#include "disjoinset.h"

#include <algorithm>
#include <limits>
#include <deque>
#include <string>

UndirectedSimpleGraph::UndirectedSimpleGraph()
{
	m_Vertices.max_load_factor(0.6);
}

bool UndirectedSimpleGraph::addVertex(const std::string& name)
{
	auto it = m_Vertices.find(name);
	if (it != m_Vertices.end())
		return false;

	m_Vertices[name] = std::unordered_set<std::string>();
	m_Vertices[name].max_load_factor(0.6);

	return true;
}

bool UndirectedSimpleGraph::removeVertex(const std::string& name)
{	
	auto it = m_Vertices.find(name);
	if (it == m_Vertices.end())
		return false;

	auto& edges = it->second;
	for (auto it2 = edges.begin(); it2 != edges.end(); ++it2)
	{
		std::string secondVertex = 
			m_Edges[*it2].m_Vertex1 == name ? 
				m_Edges[*it2].m_Vertex1 
				: 
				m_Edges[*it2].m_Vertex2;

		m_Vertices[secondVertex].erase(it->first);
	}

	m_Vertices.erase(it);
	return true;
}

int UndirectedSimpleGraph::getVertexDegree(const std::string& id) const
{
	auto it = m_Vertices.find(id);
	if (it != m_Vertices.end())
		return -1;

	return it->second.size();
}

bool UndirectedSimpleGraph::addEdge
(
	const std::string& vertexId1, 
	const std::string& vertexId2, 
	const std::string& name,
	const int& weight
)
{
	// only one edge between 2 vertices and no self loop in a simple graph
	if (vertexId1 == vertexId2)
		return false;
	else if (m_Edges.find(name) != m_Edges.end())
		return false;
	else if 
	(
		m_Vertices.find(vertexId1) == m_Vertices.end()
		||
		m_Vertices.find(vertexId2) == m_Vertices.end()
	)
		return false;
	else if (areAdjacent(vertexId2, vertexId1))
		return false;

	m_Edges[name].m_Vertex1 = vertexId1;
	m_Edges[name].m_Vertex2 = vertexId2;
	m_Edges[name].m_Weight = weight;

	m_Vertices[vertexId1].insert(name);
	m_Vertices[vertexId2].insert(name);

	return true;
}
bool UndirectedSimpleGraph::removeEdge(const std::string& edgeId)
{
	auto it = m_Edges.find(edgeId);
	if (it != m_Edges.end())
		return false;	

	auto& edgeInfo = it->second;
	m_Vertices[edgeInfo.m_Vertex1].erase(edgeId);
	m_Vertices[edgeInfo.m_Vertex2].erase(edgeId);

	m_Edges.erase(it);
	return true;
}

bool UndirectedSimpleGraph::areAdjacent(
	const std::string& vertexId1, 
	const std::string& vertexId2
) const
{
	std::string source(vertexId1), target(vertexId2);

	if (getVertexDegree(source) > getVertexDegree(target))
		std::swap(source, target);

	auto it = m_Vertices.find(source);
	if (it == m_Vertices.end())
		return false;

	const auto& edges = it->second;
	for (auto it2 = edges.begin(); it2 != edges.end(); ++it2)
	{
		auto it3 = m_Edges.find(*it2);
		if (it3 == m_Edges.end())
			continue;

		const EdgeInfo& info = it3->second;
		std::string adjacentVertex = info.m_Vertex1 == source ? info.m_Vertex2 : info.m_Vertex1;

		if (adjacentVertex == target)
			return true;
	}

	return false;
}

std::unordered_set<std::string> UndirectedSimpleGraph::getIncidentEdges(
	const std::string& vertexId
) const
{
	std::unordered_set<std::string> ret;

	auto it = m_Vertices.find(vertexId);
	if (it == m_Vertices.end())
		return ret;

	ret = it->second;

	return ret;
}

std::ostream& operator << (std::ostream& os, const UndirectedSimpleGraph& g)
{
	os << "Vertices" << std::endl;
	for (auto vertex : g.m_Vertices)
	{
		std::cout<<vertex.first<<": ";
		for (auto edge : vertex.second)
		{
			std::cout<<edge<<" ";
		}
		std::cout<<std::endl;
	}

	os << "Edges" << std::endl;
	for (auto edge : g.m_Edges)
	{
		std::cout
			<< edge.first << ": "
			<< edge.second.m_Vertex1 << " "
			<< edge.second.m_Vertex2 << " "
			<< edge.second.m_Weight
			<< std::endl;
	}
	std::cout << std::endl;
	
	return os;
}

bool UndirectedSimpleGraph::containsVertex(const std::string& name) const
{
	return m_Vertices.find(name) != m_Vertices.end();
}
 
bool UndirectedSimpleGraph::containsEdge(const std::string& name) const
{
	return m_Edges.find(name) != m_Edges.end();
}

int UndirectedSimpleGraph::getVertexNumber() const
{
	return m_Vertices.size();
}

int UndirectedSimpleGraph::getEdgeNumber() const
{
	return m_Edges.size();
}

UndirectedSimpleGraph UndirectedSimpleGraph::getKruskalMinimumSpanningTree()
{
	UndirectedSimpleGraph ret;
	std::vector<std::string> v;

	for (auto edge : m_Edges)
		v.push_back(edge.first);

	const auto& edges = m_Edges;
	std::make_heap
	(
		v.begin(), 
		v.end(),
		[&] (const std::string& a, const std::string& b)
		{
			auto it = edges.find(a);
			auto it2 = edges.find(b);

			if (it == edges.end())
				return false;
			else if (it2 == edges.end())
				return true;
			else
				return it->second.m_Weight > it2->second.m_Weight;
		}
	);

	DisjoinSet<std::string> dSet;
	for (auto vertex : m_Vertices)
		dSet.makeSet(vertex.first);

	while 
	(
		ret.getEdgeNumber() < (getVertexNumber() - 1)
		&&
		!v.empty()
	)
	{	
		std::string edgeId = v.front();
		std::pop_heap
		(
		 	v.begin(), 
		 	v.end(),
		 	[&] (const std::string& a, const std::string& b)
			{
				auto it = edges.find(a);
				auto it2 = edges.find(b);

				if (it == edges.end())
					return false;
				else if (it2 == edges.end())
						return true;
				else
					return it->second.m_Weight > it2->second.m_Weight;
			}
		);
		v.pop_back();

		auto itEdge = m_Edges.find(edgeId);

		if (dSet.find(itEdge->second.m_Vertex1) != dSet.find(itEdge->second.m_Vertex2))
		{
			dSet.setUnion(itEdge->second.m_Vertex1, itEdge->second.m_Vertex2);
			ret.addVertex(itEdge->second.m_Vertex1);
			ret.addVertex(itEdge->second.m_Vertex2);
			
			ret.addEdge
			(
				itEdge->second.m_Vertex1, 
				itEdge->second.m_Vertex2, 
				itEdge->first,
				itEdge->second.m_Weight
			);
		}
	}

	return ret;
}

UndirectedSimpleGraph UndirectedSimpleGraph::getPrimsMinimumSpanningTree(
	const std::string & start
)
{
	UndirectedSimpleGraph ret;

	std::unordered_map<std::string, std::string> predecessors;
	std::unordered_map<std::string, int> weights;
	std::unordered_map<std::string, std::string> names;

	auto comp = [&](const std::string& a, const std::string& b)  
	{
		return weights.at(a) > weights.at(b); 
	};
			
	std::deque<std::string> minHead;
	for (auto vertex : m_Vertices)
	{
		minHead.push_back(vertex.first);
		weights[vertex.first] = std::numeric_limits<int>::max();
	}
	
	weights[start] = 0;
	while (!minHead.empty())
	{
		// Decrease Key to reduce complexity here
		std::make_heap(minHead.begin(), minHead.end(),comp);

		auto current = minHead.front();
		minHead.pop_front();

		auto it = m_Vertices.find(current);
		if (it == m_Vertices.end())
			continue;

		ret.addVertex(current);

		if (predecessors.count(current))
			ret.addEdge(current, predecessors[current], names[current], weights[current]);

		for (auto itCurVertexEdge = it->second.begin(); itCurVertexEdge != it->second.end(); ++itCurVertexEdge)
		{
			auto itEdge = m_Edges.find(*itCurVertexEdge);
			if (itEdge == m_Edges.end())
				continue;

			const EdgeInfo& info = itEdge->second;
			std::string other = info.m_Vertex1 == current ? info.m_Vertex2 : info.m_Vertex1;

			if (ret.containsVertex(other))
				continue;

			auto itWeight = weights.find(other);
			if (itWeight->second > info.m_Weight)
			{
				weights[other] = info.m_Weight;		
				names[other] = itEdge->first;		
				predecessors[other] = current;
			}
		}
	}

	return ret;
}

UndirectedSimpleGraph UndirectedSimpleGraph::getDijkstraShortestPaths(
	const std::string & start
)
{
	UndirectedSimpleGraph ret;

	std::unordered_map<std::string, std::string> predecessors;
	std::unordered_map<std::string, int> weights;
	std::unordered_map<std::string, std::pair<std::string,int>> edges;

	auto comp = [&](const std::string& a, const std::string& b)  
	{
		return weights.at(a) > weights.at(b); 
	};
			
	std::deque<std::string> minHead;
	for (auto vertex : m_Vertices)
	{
		minHead.push_back(vertex.first);
		weights[vertex.first] = std::numeric_limits<int>::max();
	}
	
	weights[start] = 0;
	while (!minHead.empty())
	{
		// Decrease Key to reduce complexity here
		std::make_heap( minHead.begin(), minHead.end(), comp);
		auto current = minHead.front();
		minHead.pop_front();

		auto it = m_Vertices.find(current);
		if (it == m_Vertices.end())
			continue;

		ret.addVertex(current);

		if (predecessors.count(current))
			ret.addEdge(current, predecessors[current], edges[current].first, edges[current].second);

		for (auto itCurVertexEdge = it->second.begin(); itCurVertexEdge != it->second.end(); ++itCurVertexEdge)
		{
			auto itEdge = m_Edges.find(*itCurVertexEdge);
			if (itEdge == m_Edges.end())
				continue;

			const EdgeInfo& info = itEdge->second;
			std::string other = info.m_Vertex1 == current ? info.m_Vertex2 : info.m_Vertex1;

			if (ret.containsVertex(other))
				continue;

			int vertexWeight = weights.at(other);
			int nodeWeight = weights.at(current) + info.m_Weight;  
			
			if (vertexWeight > nodeWeight)
			{
				weights[other] = nodeWeight;
				edges[other] = make_pair(itEdge->first, info.m_Weight);		
				predecessors[other] = current;
			}
		}
	}

	return ret;
}