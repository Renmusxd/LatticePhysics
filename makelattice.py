#!/usr/bin/env python

import sys

def make_cubic(xs, ys, zs, d, m, q, k, gamma):
    to_write = []
    xs, ys, zs = int(xs), int(ys), int(zs)
    d, m, q, k, y = float(d), float(m), float(q), float(k), float(gamma)
    n_masses = xs * ys * zs

    to_write.append(str(n_masses))

    index_dict = {}
    index = 0
    for x in range(xs):
        for y in range(ys):
            for z in range(zs):
                index_dict[(x,y,z)] = index
                index += 1
                to_write.append("MASS {0} {1} {2} {3} {4} {5} {6} {7}".format(
                        x*d, y*d, z*d, 0, 0, 0, m, q))
    index = 0
    for x in range(xs):
        for y in range(ys):
            for z in range(zs):
                adj_indxs = []
                adj_indxs.append( index_dict.get((x+1,y,z)) )
                adj_indxs.append( index_dict.get((x-1,y,z)) )
                adj_indxs.append( index_dict.get((x,y+1,z)) )
                adj_indxs.append( index_dict.get((x,y-1,z)) )
                adj_indxs.append( index_dict.get((x,y,z+1)) )
                adj_indxs.append( index_dict.get((x,y,z-1)) )
                for adj_indx in adj_indxs:
                    if adj_indx:
                        to_write.append("SPRING {0} {1} {2} {3} {4}".format(
                            index,adj_indx,d,k,gamma))
                index += 1
    return to_write

def make_cubic_kick_line(xs, ys, zs, d, m, q, k, gamma, kx, ky, kz):
    """
    Kicks the x=0 line along the z axis
    """
    to_write = []
    xs, ys, zs = int(xs), int(ys), int(zs)
    d, m, q, k, y = float(d), float(m), float(q), float(k), float(gamma)
    n_masses = xs * ys * zs

    to_write.append(str(n_masses))

    index_dict = {}
    index = 0
    for x in range(xs):
        for y in range(ys):
            for z in range(zs):
                index_dict[(x,y,z)] = index
                index += 1
                kkx = 0 if x!=0 else kx
                kky = 0 if x!=0 else ky
                kkz = 0 if x!=0 else kz
                to_write.append("MASS {0} {1} {2} {3} {4} {5} {6} {7}".format(
                        x*d, y*d, z*d, kkx, kky, kkz, m, q))
    index = 0
    for x in range(xs):
        for y in range(ys):
            for z in range(zs):
                adj_indxs = []
                adj_indxs.append( index_dict.get((x+1,y,z)) )
                adj_indxs.append( index_dict.get((x-1,y,z)) )
                adj_indxs.append( index_dict.get((x,y+1,z)) )
                adj_indxs.append( index_dict.get((x,y-1,z)) )
                adj_indxs.append( index_dict.get((x,y,z+1)) )
                adj_indxs.append( index_dict.get((x,y,z-1)) )
                for adj_indx in adj_indxs:
                    if adj_indx:
                        to_write.append("SPRING {0} {1} {2} {3} {4}".format(
                            index,adj_indx,d,k,gamma))
                index += 1
    return to_write

# User input

geometries = ["cubic", "cubickick"]
params = ["x y z d m q k y", "x y z d m q k y kx ky kz"]
funcs = [make_cubic, make_cubic_kick_line]

if __name__ == "__main__":
    if len(sys.argv) <= 2:
        print("Usage: python makelattice.py " +
              "<outputfile> <geometry> [parameter list]")
        print("Geometries:")
        for i in range(len(geometries)):
            print(geometries[i] + "\t" + params[i])
        sys.exit(1)
    outputfile = sys.argv[1]
    geometry = sys.argv[2]
    for i in range(len(geometries)):
        if geometry == geometries[i]:
            n_params = len(sys.argv) - 3
            r_params = len(params[i].split())
            if n_params != r_params:
                print(geometry+" requires "+str(r_params)+" parameters:")
                print(geometry+" "+params[i])
                sys.exit(1)
            to_write = []
            if r_params > 0:
                to_write = funcs[i](*sys.argv[3:])
            else:
                to_write = funcs[i]()
            with open(outputfile, "w") as f:
                for line in to_write:
                    f.write(line + "\n")
