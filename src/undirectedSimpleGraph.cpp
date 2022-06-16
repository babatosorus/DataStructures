#include "undirectedSimpleGraph.h"

#include "disjoinset.h"

#include <algorithm>
#include <limits>
#include <deque>
#include <string>

/*
UndirectedUnweightedSimpleGraph::Edge getEdge(uint a, uint b)
{
	if (a > b)
		std::swap(a, b);

	return std::make_pair(a, b);
}

UndirectedUnweightedSimpleGraph breadthFirstTraversal 
(
	const UndirectedUnweightedSimpleGraph& graph,
	const uint& start,
	UndirectedUnweightedSimpleGraph::VertexSet& discoveredVertices,
	UndirectedUnweightedSimpleGraph::EdgeSet& discoveredEdges,
	UndirectedUnweightedSimpleGraph::EdgeSet& crossEdges
)
{
	UndirectedUnweightedSimpleGraph ret(graph.getVertexNumber());

	std::queue<uint> vQueue;
	vQueue.push(start);

	if (!discoveredVertices.count(start))		
		discoveredVertices.insert(start);

	while (!vQueue.empty())
	{
		uint curIndex = vQueue.front();
		vQueue.pop();

		auto neighbords = graph.at(curIndex);
		for (auto neighbord : neighbords)
		{
			UndirectedUnweightedSimpleGraph::Edge e = getEdge(neighbord, curIndex);
			 if (discoveredVertices.find(neighbord) == discoveredVertices.end())
			 {
			 	vQueue.push(neighbord);	 		 	
			 	discoveredVertices.insert(neighbord);
			 	discoveredEdges.insert(e);

			 	ret.createEdge(neighbord, curIndex);
			 } 
			 else if (discoveredEdges.find(e) == discoveredEdges.end())
			 {
		 		crossEdges.insert(e);
			 }
		}
	}

	return ret;
}

void deepFirstTraversal 
(
	const UndirectedUnweightedSimpleGraph& graph,
	const uint& curIndex,
	UndirectedUnweightedSimpleGraph::VertexSet& discoveredVertices,
	UndirectedUnweightedSimpleGraph::EdgeSet& discoveredEdges,
	UndirectedUnweightedSimpleGraph::EdgeSet& backEdges
)
{
	discoveredVertices.insert(curIndex);
	auto neighbords = graph.at(curIndex);

	for (auto neighbord : neighbords)
	{
		UndirectedUnweightedSimpleGraph::Edge e = getEdge(neighbord, curIndex);

		if (discoveredVertices.find(neighbord) == discoveredVertices.end())
		{
		 	discoveredVertices.insert(neighbord);
		 	discoveredEdges.insert(e);
		 	//ret.createEdge(neighbord, curIndex);
		 	deepFirstTraversal
		 	(
		 		graph,
		 		neighbord,
		 		discoveredVertices,
		 		discoveredEdges,
		 		backEdges
		 	);
		} 
		else if (discoveredEdges.find(e) == discoveredEdges.end())
		{
	 		backEdges.insert(e);
		}
	}
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
UndirectedUnweightedSimpleGraph::UndirectedUnweightedSimpleGraph( const uint& size)
: m_Vertices(size)
{
}

std::list<uint>& UndirectedUnweightedSimpleGraph::operator [] (const uint& index)
{
	if (index < 0 || index >= m_Vertices.size())
	 	throw std::range_error("vertex index out of range");

	return m_Vertices[index];
}

const std::list<uint>& UndirectedUnweightedSimpleGraph::at(const uint& index) const
{
	if (index < 0 || index >= m_Vertices.size())
	 	throw std::range_error("vertex index out of range");

	return m_Vertices[index];
}

int UndirectedUnweightedSimpleGraph::getVertexNumber() const
{
	return m_Vertices.size();
}

int UndirectedUnweightedSimpleGraph::getEdgeNumber() const
{
	return 0;
}

std::ostream& operator << (std::ostream& os, const UndirectedUnweightedSimpleGraph& g)
{
	for (uint i = 0; i < g.m_Vertices.size(); ++i)
	{
		os<< i << " : ";
		auto& neighbords = g.m_Vertices[i];
		for (auto it = neighbords.begin(); it != neighbords.end(); ++it)
			os<< *it <<" ";

		os<<std::endl;
	}

	return os;
}

std::list<uint> UndirectedUnweightedSimpleGraph::findShortestPath
(
	const uint& start, 
	const uint& end
)
{
	std::unordered_map<uint, uint> pred;
	std::unordered_set<uint> visited;
	std::queue<uint> vQueue;

	vQueue.push(start);
	visited.insert(start);

	if (start == end)
		return {start};

	bool found = false;
	while (!vQueue.empty() && !found)
	{
		uint curIndex = vQueue.front();
		vQueue.pop();

		auto neighbords = m_Vertices[curIndex];
		for (auto neighbord : neighbords)
		{
			auto it = visited.find(neighbord);
			if (it == visited.end())
			{
				vQueue.push(neighbord);
				visited.insert(neighbord);
				pred[neighbord] = curIndex;
			
				if (neighbord == end)
				{
					found = true;
					break;
				}
			}				
		}
	}

	std::list<uint> ret;
	if (found == true)
	{
		int currentIndex = end;
		ret.push_front(end);
		while (pred.count(currentIndex))
		{
			currentIndex = pred[currentIndex];
			ret.push_front(currentIndex);
		}
	}

	return ret;
}

UndirectedUnweightedSimpleGraph UndirectedUnweightedSimpleGraph::getSpanningTree(
	const uint& start
)
{
	VertexSet discoveredVertices;
	EdgeSet discoveredEdges;
	EdgeSet crossEdges;
	
	return breadthFirstTraversal
	(
		*this, 
		start, 
		discoveredVertices,
		discoveredEdges,
		crossEdges
	);
}

bool UndirectedUnweightedSimpleGraph::createEdge(const uint& v1, const uint& v2)
{
	if (v1 <0 || v1 >= m_Vertices.size())
		return false;
	else if (v2 < 0 || v2 >= m_Vertices.size())
		return false;

	m_Vertices[v1].push_back(v2);
	m_Vertices[v2].push_back(v1);

	return true;
}

std::list<UndirectedUnweightedSimpleGraph> UndirectedUnweightedSimpleGraph::getDisjointGraphs() const
{
	VertexSet discoveredVertices;
	EdgeSet discoveredEdges;
	EdgeSet crossEdges;

	std::list<UndirectedUnweightedSimpleGraph> ret;

	for (uint i = 0; i <m_Vertices.size(); ++i)
	{
		if (!discoveredVertices.count(i))
			ret.push_back
			(
				breadthFirstTraversal
				(
					*this, 
					i, 
					discoveredVertices,
					discoveredEdges,
					crossEdges
				)
			);
	}

	return ret;
}

bool UndirectedUnweightedSimpleGraph::hasLoop() const
{
	VertexSet discoveredVertices;
	EdgeSet discoveredEdges;
	EdgeSet backEdges;

	for (uint i = 0; i < m_Vertices.size(); ++i)
	{
		if (!discoveredVertices.count(i))
		{
			//printf("restart recursion\n");
			deepFirstTraversal
			(
				*this,
				i,
				discoveredVertices,
				discoveredEdges,
				backEdges
			);
		}
	}

	//std::cout<<"size =="<<backEdges.size()<<std::endl;
	return !backEdges.empty(); 
}
*/

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------

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

UndirectedSimpleGraph UndirectedSimpleGraph::getPrimsMinimumSpanningTree
(
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

UndirectedSimpleGraph UndirectedSimpleGraph::getDijkstraShortestPaths
(
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