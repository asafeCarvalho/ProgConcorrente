import random

def create_random_graphs(low_num_vertices, high_num_vertices, filename, limit_multiplier=3, amount=100, min_weight=1, max_weight=100):
    """
    Generates multiple random directed weighted graphs and saves them to a file.
    Graphs are guaranteed to be connected via a spanning tree, then further randomized.

    Args:
        low_num_vertices (int): Minimum number of vertices for a graph.
        high_num_vertices (int): Maximum number of vertices for a graph.
        filename (str): The name of the file to save the graphs.
        limit_multiplier (int): Multiplier for the maximum number of edges in MCMC.
                                 For directed graphs, actual limit is num_vertices * limit_multiplier.
                                 For undirected, it's half that.
        amount (int): The number of random graphs to generate.
        min_weight (int): Minimum weight for an edge.
        max_weight (int): Maximum weight for an edge.
    """
    iterations = 100000 # Number of MCMC iterations to refine graph
    with open(filename, 'w') as f:
        print(amount, file=f) # First line in file indicates number of graphs
        for _ in range(amount):
            num_vertices = random.randint(low_num_vertices, high_num_vertices)
            # Graph will always be directed (1) as requested.
            directed = 1 
            # Adjacency list: each element is a dictionary mapping neighbor -> weight
            adjacency_list = [{} for _ in range(num_vertices)]
            
            # Always create a spanning tree to ensure initial connectivity
            create_spanning_tree(num_vertices, directed, adjacency_list, min_weight, max_weight)

            # Apply MCMC to further randomize graph while maintaining connectivity and edge limits
            mcmc(iterations, num_vertices, directed, limit_multiplier, adjacency_list, min_weight, max_weight)
            
            # Transform to edge list format for writing to file
            edge_list = transform_graph_in_edge_list(num_vertices, adjacency_list)
            
            # Write the graph to the file
            put_graph_in_file(num_vertices, edge_list, f, directed)

def create_random_graph(num_vertices, limit_multiplier=3, min_weight=1, max_weight=100):
    """
    Generates a single random directed weighted graph and returns its edge list.
    The graph is guaranteed to be connected via a spanning tree, then further randomized.

    Args:
        num_vertices (int): The number of vertices for the graph.
        limit_multiplier (int): Multiplier for the maximum number of edges in MCMC.
        min_weight (int): Minimum weight for an edge.
        max_weight (int): Maximum weight for an edge.

    Returns:
        list: A list of tuples, where each tuple represents an edge (source, destination, weight).
    """
    iterations = 100000 # Number of MCMC iterations
    # Graph will always be directed (1) as requested.
    directed = 1
    # Adjacency list: each element is a dictionary mapping neighbor -> weight
    adjacency_list = [{} for _ in range(num_vertices)]
    
    # Always create a spanning tree to ensure connectivity
    create_spanning_tree(num_vertices, directed, adjacency_list, min_weight, max_weight)

    # Apply MCMC to further randomize graph while maintaining connectivity and edge limits
    mcmc(iterations, num_vertices, directed, limit_multiplier, adjacency_list, min_weight, max_weight)
    
    # Transform to edge list format
    edge_list = transform_graph_in_edge_list(num_vertices, adjacency_list)
    return edge_list


def create_spanning_tree(num_vertices, directed, adjacency_list, min_weight, max_weight):
    """
    Creates a random spanning tree, ensuring the graph is connected.

    Args:
        num_vertices (int): The number of vertices in the graph.
        directed (int): 0 for undirected, 1 for directed.
        adjacency_list (list): The graph's adjacency list.
        min_weight (int): Minimum weight for an edge.
        max_weight (int): Maximum weight for an edge.
    """
    # List of vertices not yet in the tree
    not_in_tree = list(range(1, num_vertices))
    random.shuffle(not_in_tree)
    
    # List of vertices already in the tree (start with vertex 0)
    in_tree = [0] 

    for _ in range(num_vertices - 1): # A spanning tree has num_vertices - 1 edges
        # Choose a random vertex already in the tree
        u = random.choice(in_tree)
        # Choose a random vertex not yet in the tree
        v = not_in_tree.pop()
        
        weight = random.randint(min_weight, max_weight)
        add_edge(u, v, directed, adjacency_list, weight)
        in_tree.append(v) # Add the new vertex to the tree


def dfs(current_vertex, adjacency_list, visited):
    """
    Depth-First Search (DFS) for connectivity check.

    Args:
        current_vertex (int): The current vertex being visited.
        adjacency_list (list): The graph's adjacency list.
        visited (list): A boolean list to track visited vertices.
    """
    visited[current_vertex] = True
    # Iterate through neighbors (keys of the dictionary)
    for neighbor in adjacency_list[current_vertex]:
        if not visited[neighbor]:
            dfs(neighbor, adjacency_list, visited)


def check_conectivity(num_vertices, adjacency_list):
    """
    Checks the connectivity of the graph using DFS.
    Used in MCMC to prevent disconnecting the graph when removing edges.

    Args:
        num_vertices (int): The number of vertices in the graph.
        adjacency_list (list): The graph's adjacency list.

    Returns:
        bool: True if the graph is connected, False otherwise.
    """
    visited = [False for _ in range(num_vertices)]
    
    # Perform DFS from vertex 0 (assuming connected graphs from spanning tree start)
    dfs(0, adjacency_list, visited)
    
    # If any vertex was not visited, the graph is not connected
    for i in range(num_vertices):
        if not visited[i]:
            return False
    return True

def add_edge(u, v, directed, adjacency_list, weight):
    """
    Adds an edge (u, v) with a given weight to the adjacency list.

    Args:
        u (int): Source vertex.
        v (int): Destination vertex.
        directed (int): 0 for undirected, 1 for directed.
        adjacency_list (list): The graph's adjacency list.
        weight (int): The weight of the edge.
    """
    adjacency_list[u][v] = weight
    if not directed:
        # This block will no longer be executed as 'directed' is always 1
        adjacency_list[v][u] = weight

def remove_edge(u, v, directed, adjacency_list):
    """
    Removes an edge (u, v) from the adjacency list.

    Args:
        u (int): Source vertex.
        v (int): Destination vertex.
        directed (int): 0 for undirected, 1 for directed.
        adjacency_list (list): The graph's adjacency list.
    """
    if v in adjacency_list[u]:
        del adjacency_list[u][v]
    if not directed and u in adjacency_list[v]:
        # This block will no longer be executed as 'directed' is always 1
        del adjacency_list[v][u]

def mcmc(iterations, num_vertices, directed, max_edges_multiplier, adjacency_list, min_weight, max_weight):
    """
    Monte Carlo Markov Chain (MCMC) algorithm to generate random connected graphs.
    It adds/removes edges to randomize the graph while maintaining connectivity and
    respecting a maximum edge limit.

    Args:
        iterations (int): Number of iterations for the MCMC process.
        num_vertices (int): The number of vertices in the graph.
        directed (int): 0 for undirected, 1 for directed.
        max_edges_multiplier (int): Multiplier for the maximum number of edges.
        adjacency_list (list): The graph's adjacency list.
        min_weight (int): Minimum weight for a new edge.
        max_weight (int): Maximum weight for a new edge.
    """
    current_edges_count = sum(len(adj) for adj in adjacency_list)
    if not directed:
        current_edges_count //= 2 # For undirected, each edge is counted twice (this will no longer be executed)

    # Calculate max_edges_limit based on directed/undirected
    # Since 'directed' is always 1, only the 'if directed' branch will be relevant.
    if directed:
        max_edges_limit = num_vertices * max_edges_multiplier
    else:
        max_edges_limit = (num_vertices * max_edges_multiplier) // 2 
        max_edges_limit = max(max_edges_limit, num_vertices - 1)


    for _ in range(iterations):
        u = random.randint(0, num_vertices - 1)
        v = random.randint(0, num_vertices - 1)
        
        if u == v: # Avoid self-loops
            continue

        if v in adjacency_list[u]: # Edge (u,v) exists, try to remove it
            
            # Temporarily remove the edge
            original_weight_uv = adjacency_list[u].get(v)
            # original_weight_vu will always be None for directed graphs
            original_weight_vu = adjacency_list[v].get(u) if not directed else None 
            remove_edge(u, v, directed, adjacency_list)
            
            # Check if graph remains connected after removal
            if not check_conectivity(num_vertices, adjacency_list):
                # If not connected, revert the removal
                add_edge(u, v, directed, adjacency_list, original_weight_uv)
                # This 'if not directed' block will not be executed
                if not directed and original_weight_vu is not None:
                    add_edge(v, u, directed, adjacency_list, original_weight_vu)
            else:
                current_edges_count -= 1

        else: # Edge (u,v) does not exist, try to add it
            if current_edges_count >= max_edges_limit:
                continue # Do not add if max edge limit is reached
            
            weight = random.randint(min_weight, max_weight)
            add_edge(u, v, directed, adjacency_list, weight)
            current_edges_count += 1
            

def transform_graph_in_edge_list(num_vertices, adjacency_list):
    """
    Transforms the adjacency list representation into a list of edges.
    Each edge is represented as a tuple (source, destination, weight).

    Args:
        num_vertices (int): The number of vertices in the graph.
        adjacency_list (list): The graph's adjacency list.

    Returns:
        list: A list of tuples (u, v, weight).
    """
    edge_list = []
    # For directed graphs, seen_edges is less critical but still good practice to prevent
    # accidental duplicates if logic were to change.
    seen_edges = set() 
    
    for u in range(num_vertices):
        for v, weight in adjacency_list[u].items():
            # For directed graphs, (v, u) will generally not be in seen_edges unless added explicitly
            # by mistake or if processing an undirected graph with this function.
            # Given 'directed' is now always 1, this check is less active for actual edge generation.
            if (v, u) in seen_edges: 
                continue
            
            edge_list.append((u + 1, v + 1, weight)) # +1 to make vertices 1-indexed for output
            seen_edges.add((u,v)) # Mark (u,v) as seen
    return edge_list


def put_graph_in_file(num_vertices, edge_list, f, directed):
    """
    Writes a single graph's data to the given file object.

    Args:
        num_vertices (int): The number of vertices.
        edge_list (list): List of edges (u, v, weight).
        f (file object): The file to write to.
        directed (int): 0 for undirected, 1 for directed.
    """
    # First line of a graph block: num_vertices, num_edges, directed (0/1)
    print(num_vertices, len(edge_list), directed, file=f)
    for u, v, weight in edge_list:
        # Each edge line: source_vertex, destination_vertex, weight
        print(u, v, weight, file=f)
    print(1, file=f) # Sentinel value, usually indicates end of graph data


# --- Example Usage ---

def generate_example_graphs():
    """
    Demonstrates how to use the functions to generate different types of graphs.
    All generated graphs will now be directed.
    """
    print("Generating example graph files (all directed)...")

    # Performance testing graphs (various sizes)
    create_random_graphs(5, 5, "random_weighted_graphs/small_5_vertices.txt", 3, 10, min_weight=1, max_weight=50)
    create_random_graphs(10, 10, "random_weighted_graphs/medium_10_vertices.txt", 3, 10, min_weight=1, max_weight=50)
    create_random_graphs(15, 15, "random_weighted_graphs/large_15_vertices.txt", 3, 10, min_weight=1, max_weight=50)
    
    # Graphs for general connectivity testing
    create_random_graphs(5, 10, "random_weighted_graphs/random_connected_graphs.txt", 3, 5, min_weight=1, max_weight=50)
    # Graphs with fewer edges (closer to a tree structure)
    create_random_graphs(5, 10, "random_weighted_graphs/sparse_graphs.txt", 0, 5, min_weight=1, max_weight=50) 

    print("Example graph files generated in 'random_weighted_graphs/' directory.")
    print("You might need to create the 'random_weighted_graphs' directory manually.")

# To run the example generation, uncomment the line below:
# generate_example_graphs()

# Example of generating a single graph and printing its edge list:
if __name__ == "__main__":
    # print("\n--- Generating a single random directed weighted graph ---")
    # Generate a directed weighted graph with 6 vertices
    # The 'directed' parameter is no longer explicitly passed to create_random_graph as it's fixed to 1.
    vertices = 100000
    menor_peso = -100000000
    maior_peso = menor_peso * -1

    single_graph_edges = create_random_graph(num_vertices=vertices, limit_multiplier=2, min_weight=menor_peso, max_weight=maior_peso)
    # print(f"Number of vertices: 6, Directed: True")
    # print("Edges (source, destination, weight):")
    print(f"{vertices} {len(single_graph_edges)}");
    for edge in single_graph_edges:
        print(f"{edge[0]} {edge[1]} {edge[2]}")

    # Removed the example for generating an undirected graph as per your request
    # print("\n--- Generating a single random undirected weighted graph ---")
    # undirected_graph_edges = create_random_graph(num_vertices=5, directed=0, limit_multiplier=3, min_weight=1, max_weight=10)
    # print(f"Number of vertices: 5, Directed: False")
    # print("Edges (source, destination, weight):")
    # for edge in undirected_graph_edges:
    #     print(edge)
