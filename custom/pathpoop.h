//
// Created by dev on 2/21/2024.
//

#ifndef VCSI_SARDINOS_PATHPOOP_H
#define VCSI_SARDINOS_PATHPOOP_H
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <set>
#include <cstdlib>
#include <ctime>
#include <QGeoCoordinate>
#include <vector>
#include <algorithm>

struct DisjointSets {
    std::vector<int> parent, rank;
    int n;

    DisjointSets(int n) : n(n), parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int u) {
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    void merge(int x, int y) {
        x = find(x), y = find(y);
        if (rank[x] > rank[y])
            parent[y] = x;
        else
            parent[x] = y;
        if (rank[x] == rank[y])
            rank[y]++;
    }
};

// Edge structure to store edges along with their weights
struct Edge {
    int src, dest;
    double weight;
    Edge(int src, int dest, double weight) : src(src), dest(dest), weight(weight) {}
};

// Comparator function to sort edges based on their weight
bool compareEdge(const Edge& e1, const Edge& e2) {
    return e1.weight < e2.weight;
}

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

namespace poop {
    std::vector<Point> convertGeoCoordinatesToPoints(const std::vector<std::vector<QGeoCoordinate>>& geoCoordinates) {
        std::vector<Point> points;
        for (const auto& vec : geoCoordinates) {
            for (const auto& coord : vec) {
                points.emplace_back(coord.longitude(), coord.latitude());
            }
        }
        return points;
    }

    std::vector<std::vector<QGeoCoordinate>> convertPointsToGeoCoordinates(const std::vector<Point>& points, const std::vector<std::pair<int, int>>& edges) {
        std::vector<std::vector<QGeoCoordinate>> res;
        for (const auto& edge : edges) {
            const Point& start = points[edge.first];
            const Point& end = points[edge.second];
            QGeoCoordinate A = {start.y, start.x};
            QGeoCoordinate B = {end.y, end.x};
            res.push_back({A, B});
        }

        return res;
    }


    bool isInsidePolygon(const std::vector<Point>& polygon, const Point& point) {
        bool result = false;
        int n = polygon.size();
        for (int i = 0, j = n - 1; i < n; j = i++) {
            if ((polygon[i].y > point.y) != (polygon[j].y > point.y) &&
                (point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
                result = !result;
            }
        }
        return result;
    }

    double distance(const Point& a, const Point& b) {
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    }

    std::vector<Point> generatePointsInsidePolygon(const std::vector<Point>& polygon, int resolutionX, int resolutionY) {
        std::vector<Point> points;
        double minX = 50000.0;
        double maxX = -50000.0;

        double minY = 50000.0;
        double maxY = -50000.0;

        for (const auto& p : polygon) {
            minX = (((minX) < (p.x)) ? (minX) : (p.x));
            maxX = (((maxX) > (p.x)) ? (maxX) : (p.x));
            minY = (((minY) < (p.y)) ? (minY) : (p.y));
            maxY = (((maxY) > (p.y)) ? (maxY) : (p.y));
        }

        // Generate grid
        double stepX = (maxX - minX) / resolutionX;
        double stepY = (maxY - minY) / resolutionY;
        for (int i = 0; i < resolutionX; ++i) {
            for (int j = 0; j < resolutionY; ++j) {
                Point p = {minX + i * stepX, minY + j * stepY};
                if (isInsidePolygon(polygon, p)) {
                    points.push_back(p);
                }
            }
        }

        return points;

//        std::srand(std::time(nullptr));
//        while (points.size() < count) {
//            double x = minX + (maxX - minX) * (std::rand() / (RAND_MAX + 1.0));
//            double y = minY + (maxY - minY) * (std::rand() / (RAND_MAX + 1.0));
//            Point p(x, y);
//            if (isInsidePolygon(polygon, p)) {
//                points.push_back(p);
//            }
//        }
//        return points;
    }

    std::vector<std::pair<int, int>> findSpanningTree(const std::vector<Point>& points) {
        int n = points.size();
        std::vector<std::pair<int, int>> edges; // Store edges of the spanning tree
        std::set<int> connected; // Track connected vertices
        connected.insert(0); // Start with the first point

        while (connected.size() < n) {
            double minDist = 99999;
            int closest, from;
            for (int i : connected) {
                for (int j = 0; j < n; ++j) {
                    if (connected.find(j) == connected.end()) { // Not yet connected
                        double dist = distance(points[i], points[j]);
                        if (dist < minDist) {
                            minDist = dist;
                            closest = j;
                            from = i;
                        }
                    }
                }
            }
            edges.emplace_back(from, closest);
            connected.insert(closest);
        }
        return edges;
    }

    std::vector<std::pair<int, int>> findSpanningTreeKruskal(const std::vector<Point>& points) {
        int n = points.size();
        std::vector<Edge> edges;

        // Creating all possible edges with their weights
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                double dist = distance(points[i], points[j]);
                edges.emplace_back(i, j, dist);
            }
        }

        // Sort edges based on their weight
        std::sort(edges.begin(), edges.end(), compareEdge);

        DisjointSets ds(n);

        std::vector<std::pair<int, int>> result;

        for (auto& edge : edges) {
            int set_u = ds.find(edge.src);
            int set_v = ds.find(edge.dest);

            if (set_u != set_v) {
                result.push_back({edge.src, edge.dest});
                ds.merge(set_u, set_v);
            }
        }

        return result;
    }
}

#endif  // VCSI_SARDINOS_PATHPOOP_H
