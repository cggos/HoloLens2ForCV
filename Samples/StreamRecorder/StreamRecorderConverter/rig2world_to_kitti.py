"""
1. get kitti format file after running the script
2. plot:
        evo_traj  kitti <path-to-rig2world.kitti> -p --plot_mode xz
"""
import argparse
import numpy as np
from numpy.linalg import inv

# K R from LZ

K = np.matrix(np.diag([1,1,-1,1]))

R = np.matrix(np.zeros((4,4)))
R[0,1] = 1
R[1,0] = 1
R[2,2] = 1
R[3,3] = 1

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--dir", required=True, help="the dir of xxx_rig2world.txt")
    args = parser.parse_args()

    data_dir = args.dir + "/"

    row_major = True

    if row_major:
        of_rig2world = open(data_dir + "Depth Long Throw_rig2world.txt")
        of_kitti = open(data_dir + "rig2world.kitti", 'w')
    else:
        of_rig2world = open(data_dir + "ARRuler_1637825324496.txt")
        of_kitti = open(data_dir + "arruler.kitti", 'w')

    tf_data = np.zeros(16, float)

    for line in of_rig2world:
        data = line.split(',')
        assert len(data) == 17
        if row_major:
            for i in range(1, 13):
                tf_data[i-1] = float(data[i])
        else:
            for i in range(1, 17):
                tf_data[i-1] = float(data[i])
            tf_gl = np.matrix(tf_data.reshape((4,4), order='F'))

            # tf_slam = inv(K) * inv(R) * tf_gl * R * K

            tf_data = tf_gl.A.reshape(16, order='C')

        for i in range(12):
            if i == 11:
                of_kitti.write("{}\n".format(float(tf_data[i])))
                break
            of_kitti.write("{} ".format(float(tf_data[i])))

    of_kitti.close()


if __name__ == "__main__":
    main()
