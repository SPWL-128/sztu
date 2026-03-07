import open3d as o3d
import numpy as np
import matplotlib.pyplot as plt
import copy

def crop(pcd, x_range, y_range, z_range):
    """
    从三个方向裁剪点云
    :param pcd:
    :param x_range:
    :param y_range:
    :param z_range:
    :return:
    """
    pcd_points = np.array(pcd.points)
    cropped = []
    for i in range(len(pcd_points)):
        if x_range[0] < pcd_points[i][0] < x_range[1]:
            if y_range[0] < pcd_points[i][1] < y_range[1]:
                if z_range[0] < pcd_points[i][2] < z_range[1]:
                    cropped.append(pcd_points[i])
    # print("cropped", np.array(cropped))
    cropped_np = np.array(cropped)
    new = o3d.geometry.PointCloud()
    new.points = o3d.utility.Vector3dVector(cropped_np[:, :3])
    print(new.points)
    return new

def classify(clustered_pcd, labels):
    """
    单独显示每个聚类后的点云
    :param clustered_pcd:
    :param labels:
    :return:
    """
    min = labels.min()
    max = labels.max()
    for label in range(min, max+1):
        label_index = np.where(labels == label)  # 提取分类为label的聚类点云下标
        label_pcd = clustered_pcd.select_by_index(np.array(label_index)[0])  # 根据下标提取点云点
        print('label: ', str(label), '点云数：', len(label_pcd.points))

        # 可视化
        o3d.visualization.draw_geometries([label_pcd])

def preprocess_data(pcd):           # 预处理
    pcd_transform = copy.deepcopy(pcd)
    t_matrix_x = np.eye(4)
    t_matrix_x[:3, :3] = pcd.get_rotation_matrix_from_xyz((np.radians(np.random.randint(0, 360)), 0, 0))

    t_matrix_y = np.eye(4)
    t_matrix_y[:3, :3] = pcd_transform.get_rotation_matrix_from_xyz((0, np.radians(np.random.randint(0, 360)), 0))
    pcd_transform.transform(t_matrix_y)

    t_matrix_z = np.eye(4)
    t_matrix_z[:3, :3] = pcd_transform.get_rotation_matrix_from_xyz((0, 0, np.radians(np.random.randint(0, 360))))
    pcd_transform.transform(t_matrix_z)

    pcd_transform.transform(t_matrix_x)
    pcd_transform.transform(t_matrix_y)
    pcd_transform.transform(t_matrix_z)
    o3d.visualization.draw_geometries([pcd, pcd_transform])     # 显示变换
    return pcd_transform


def get_fpfh(pcd):  # 获得fpfh参数
    radius_normal = max(0.05, calculate_point_density(pcd) * 3)
    print(":: Estimate normal with search radius %.3f." % radius_normal)
    pcd.estimate_normals(
        o3d.geometry.KDTreeSearchParamHybrid(radius=radius_normal, max_nn=50))

    radius_feature = max(0.15, radius_normal * 3)  # 至少0.15，覆盖螺旋范围
    print(":: Compute FPFH feature with search radius %.3f." % radius_feature)
    pcd_fpfh = o3d.pipelines.registration.compute_fpfh_feature(
        pcd,
        o3d.geometry.KDTreeSearchParamHybrid(radius=radius_feature, max_nn=200))
    return pcd_fpfh

def calculate_point_density(pcd):
    """动态计算点云密度"""
    if len(pcd.points) < 100:
        return 0.05
    points = np.asarray(pcd.points)
    centroid = np.mean(points, axis=0)
    distances = np.linalg.norm(points - centroid, axis=1)
    return np.mean(distances) * 0.3

def execute_fast_global_registration(source, target, source_fpfh, target_fpfh, voxel_size):
    # 1. 合理设置距离阈值（避免为0）
    distance_threshold = max(0.01, voxel_size * 1.0)  # 使用体素大小的1倍，最小值0.01
    # 2. 设置合理的迭代次数（避免默认值太低）
    option = o3d.pipelines.registration.FastGlobalRegistrationOption(
        maximum_correspondence_distance=distance_threshold,
        iteration_number=200  # 增加迭代次数
    )
    # 3. 添加输入点云尺寸检查
    if len(source.points) < 4 or len(target.points) < 4:
        print("点云点数不足，无法执行配准！")
        return o3d.pipelines.registration.RegistrationResult(np.eye(4))

    print(f":: 应用快速全局配准，距离阈值={distance_threshold:.3f}，迭代次数={option.iteration_number}")
    # 4. 使用修正后的API调用
    result = o3d.pipelines.registration.registration_fgr_based_on_feature_matching(
        source,
        target,
        source_fpfh,
        target_fpfh,
        option=option
    )
    return result

def icp_registration(source, target, global_transformation, voxel_size):        # icp配准
    """
    :param source:
    :param target:
    :param global_transformation:
    :param voxel_size:
    :return:
    """
    distance_threshold = voxel_size * 0.4  # 距离阈值是体素大小的0.4倍
    print(":: Point-to-plane ICP registration is applied on original point")
    print("   clouds to refine the alignment. This time we use a strict")
    print("   distance threshold %.3f." % distance_threshold)
    result = o3d.pipelines.registration.registration_icp(
        source, target, distance_threshold, global_transformation,
        o3d.pipelines.registration.TransformationEstimationPointToPlane())
    return result

def draw_registration_result(source, target, transformation):       # 配准输出
    source_temp = copy.deepcopy(source)
    target_temp = copy.deepcopy(target)
    print(source_temp)
    source_temp.paint_uniform_color([0, 1, 1])
    target_temp.paint_uniform_color([1, 0, 0])
    source_temp.transform(transformation)
    all_pcd = source_temp + target_temp
    print(np.array(all_pcd.colors))
    o3d.visualization.draw_geometries([source_temp, target_temp])


def combine_point_cloud(point_clouds, spacing=20.0):
    """
    将多个点云沿X轴方向等间距排列合并

    参数:
        point_clouds: 点云对象列表(open3d.geometry.PointCloud)
        spacing: 点云之间的间距(默认20个单位)

    返回:
        combined_pcd: 合并后的点云
        arranged_pcds: 排列后的点云列表
    """
    # 深拷贝点云避免修改原始数据
    arranged_pcds = [copy.deepcopy(pcd) for pcd in point_clouds]
    combined_points = []
    combined_colors = []

    current_x = 0.0  # 当前X轴起始位置

    for i, pcd in enumerate(arranged_pcds):
        # 获取点云边界和尺寸
        min_bound = pcd.get_min_bound()
        max_bound = pcd.get_max_bound()
        width = max_bound[0] - min_bound[0]

        # 计算平移向量：确保间距一致
        translation = np.array([current_x - min_bound[0], 0, 0])

        # 应用平移变换[4,8](@ref)
        pcd.translate(translation, relative=True)

        # 更新合并点云数据
        points = np.asarray(pcd.points)
        colors = np.asarray(pcd.colors) if pcd.has_colors() else None

        combined_points.append(points)
        if colors is not None:
            combined_colors.append(colors)

        # 更新下一个点云的起始位置（当前终点 + 间距）
        current_x = np.max(points[:, 0]) + spacing

    # 创建合并点云对象
    combined_pcd = o3d.geometry.PointCloud()
    combined_pcd.points = o3d.utility.Vector3dVector(np.vstack(combined_points))

    if combined_colors:
        combined_pcd.colors = o3d.utility.Vector3dVector(np.vstack(combined_colors))

    return combined_pcd, arranged_pcds

if __name__ == '__main__':              # 步骤1 添加需要的点云图文件
    pcd1 = o3d.io.read_point_cloud(r'D:\Desktop\5.pcd')
    pcd2 = o3d.io.read_point_cloud(r'D:\Desktop\6.pcd')
    pcd3 = o3d.io.read_point_cloud(r'D:\Desktop\7.pcd')
    pcd4 = o3d.io.read_point_cloud(r'D:\Desktop\8.pcd')
    pcd5 = o3d.io.read_point_cloud(r'D:\Desktop\9.pcd')

    # o3d.visualization.draw_geometries([pcd1])

    # down sampling降采样
    down_sample1 = pcd1.voxel_down_sample(0.005)
    down_sample2 = pcd2.voxel_down_sample(0.005)
    down_sample3 = pcd3.voxel_down_sample(0.005)
    down_sample4 = pcd4.voxel_down_sample(0.005)
    down_sample5 = pcd5.voxel_down_sample(0.005)
    # o3d.visualization.draw_geometries([down_sample1])     # 降采样图

    # crop裁剪
    cropped_pcd1 = crop(down_sample1, x_range=[-150, 150], y_range=[-150, 150], z_range=[830, 926])
    cropped_pcd2 = crop(down_sample2, x_range=[-150, 150], y_range=[-150, 350], z_range=[880, 926])
    cropped_pcd3 = crop(down_sample3, x_range=[-150, 150], y_range=[-150, 400], z_range=[880, 926])
    cropped_pcd4 = crop(down_sample4, x_range=[-150, 125], y_range=[-40 ,150], z_range=[800, 933])
    cropped_pcd5 = crop(down_sample5, x_range=[-150, 210], y_range=[75, 140], z_range=[880, 925])

    # o3d.visualization.draw_geometries([cropped_pcd4])     # 裁剪图

    # remove outlier去除噪点
    remove_outlier1, index = cropped_pcd1.remove_statistical_outlier(20, 1)
    remove_outlier2, index = cropped_pcd2.remove_statistical_outlier(20, 1)
    remove_outlier3, index = cropped_pcd3.remove_statistical_outlier(20, 1)
    remove_outlier4, index = cropped_pcd4.remove_statistical_outlier(20, 1)
    remove_outlier5, index = cropped_pcd5.remove_statistical_outlier(20, 1)
    remove_outlier1.paint_uniform_color([1, 0, 0])  # 红
    remove_outlier2.paint_uniform_color([0, 1, 0])  # 绿
    remove_outlier3.paint_uniform_color([0, 0, 1])
    remove_outlier4.paint_uniform_color([0, 1, 1])
    remove_outlier5.paint_uniform_color([1, 1, 0])
    # o3d.visualization.draw_geometries([remove_outlier1])      # 去噪点图



    combined_pcd, arranged_pcds =combine_point_cloud([remove_outlier1,remove_outlier2,remove_outlier3,remove_outlier4,remove_outlier5],40)
    # cluster聚类
    with o3d.utility.VerbosityContextManager(o3d.utility.VerbosityLevel.Debug) as cm:
        labels = np.array(combined_pcd.cluster_dbscan(eps=25, min_points=8, print_progress=True))
    max_label = labels.max()
    print(f"point cloud has {max_label + 1} clusters")
    colors = plt.get_cmap("tab20")(labels / (max_label if max_label > 0 else 1))
    colors[labels < 0] = 0
    combined_pcd.colors = o3d.utility.Vector3dVector(colors[:, :3])
    o3d.visualization.draw_geometries([combined_pcd])
    classify(combined_pcd, labels)    # 提取簇

    target=remove_outlier3      # 以第三个物体为目标物体

    # 目标物体与传送带物体配准
    a1=target
    a1_fpfh=get_fpfh(a1)
    combine_fpfh=get_fpfh(combined_pcd)
    fgr=execute_fast_global_registration(a1,combined_pcd,a1_fpfh,combine_fpfh,0.05)        # 快速全局配准
    draw_registration_result(a1,combined_pcd,fgr.transformation)
    icp=icp_registration(a1,combined_pcd,fgr.transformation,0.05)     # icp配准
    draw_registration_result(a1,combined_pcd,icp.transformation)
