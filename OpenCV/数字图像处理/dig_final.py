import cv2
import numpy as np
import matplotlib.pyplot as plt

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False


# ==================== 全局变量和辅助函数 ====================

def load_image():
    """加载老照片"""
    img_path = r"D:\desktop\screenshot_20231227_222451.png"   # 修改成对应路径
    img_bgr = cv2.imread(img_path)

    if img_bgr is None:
        print(f"错误：无法读取图片，请检查路径：{img_path}")
        return None

    print(f"成功读取图片：{img_path}")
    print(f"图片尺寸：{img_bgr.shape}")

    # 转换为RGB格式（matplotlib使用RGB）
    img_rgb = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2RGB)
    return img_rgb


# ==================== 任务一：系统初始化与图像质量评估 ====================
    # 图像质量评估

def task1_quality_assessment(img_rgb):
    """任务一：系统初始化与图像质量评估"""
    # 转换为灰度图像
    gray = cv2.cvtColor(img_rgb, cv2.COLOR_RGB2GRAY)

    # 分离RGB通道
    r_channel = img_rgb[:, :, 0]
    g_channel = img_rgb[:, :, 1]
    b_channel = img_rgb[:, :, 2]

    # 计算直方图
    r_hist = cv2.calcHist([r_channel], [0], None, [256], [0, 256])
    g_hist = cv2.calcHist([g_channel], [0], None, [256], [0, 256])
    b_hist = cv2.calcHist([b_channel], [0], None, [256], [0, 256])
    gray_hist = cv2.calcHist([gray], [0], None, [256], [0, 256])

    # 创建可视化窗口
    plt.figure(figsize=(15, 8))
    plt.suptitle('任务一：系统初始化与图像质量评估', fontsize=16, fontweight='bold')

    # 原始彩色图像
    plt.subplot(2, 4, 1)
    plt.imshow(img_rgb)
    plt.title('原始老照片')
    plt.axis('off')

    # 灰度图像
    plt.subplot(2, 4, 2)
    plt.imshow(gray, cmap='gray')
    plt.title('灰度图像')
    plt.axis('off')

    # RGB通道直方图
    colors = ['red', 'green', 'blue']
    channels = [r_hist, g_hist, b_hist]
    titles = ['红色通道直方图', '绿色通道直方图', '蓝色通道直方图']

    for i in range(3):
        plt.subplot(2, 4, 3 + i)
        plt.plot(channels[i], color=colors[i], alpha=0.7)
        plt.fill_between(range(256), channels[i].flatten(), alpha=0.3, color=colors[i])
        plt.title(titles[i])
        plt.xlabel('像素值')
        plt.ylabel('频数')
        plt.grid(True, alpha=0.3)
        plt.xlim([0, 255])

    # 灰度直方图
    plt.subplot(2, 4, 7)
    plt.plot(gray_hist, 'k', alpha=0.7)
    plt.fill_between(range(256), gray_hist.flatten(), alpha=0.3, color='gray')
    plt.title('灰度直方图')
    plt.xlabel('像素值')
    plt.ylabel('频数')
    plt.grid(True, alpha=0.3)
    plt.xlim([0, 255])

    # 空位
    plt.subplot(2, 4, 8)
    plt.axis('off')

    plt.tight_layout()
    plt.show()

    return img_rgb, gray


# ==================== 任务二：空间域增强与去噪 ====================
    # 解决对比度、亮度等画面问题，并去除噪声

def task2_spatial_enhancement(gray):
    """任务二：空间域增强与去噪"""
    # 1. Gamma校正
    gamma = 0.5
    normalized = gray / 255.0
    gamma_corrected = np.power(normalized, gamma)
    gamma_corrected = (gamma_corrected * 255).astype(np.uint8)

    # 2. 直方图均衡化
    equalized = cv2.equalizeHist(gray)

    # 3. 添加椒盐噪声，模拟老照片
    noisy = gray.copy()
    noise_density = 0.05
    num_noise = int(noise_density * gray.size)
    coords = [np.random.randint(0, i - 1, num_noise) for i in gray.shape]
    num_salt = num_noise // 2
    num_pepper = num_noise - num_salt

    salt_coords = [c[:num_salt] for c in coords]
    noisy[salt_coords[0], salt_coords[1]] = 255

    pepper_coords = [c[num_salt:] for c in coords]
    noisy[pepper_coords[0], pepper_coords[1]] = 0

    # 4. 空间滤波去噪
    mean_filtered = cv2.blur(noisy, (5, 5))
    median_filtered = cv2.medianBlur(noisy, 5)

    # 5. 计算直方图
    hist_original = cv2.calcHist([gray], [0], None, [256], [0, 256])
    hist_gamma = cv2.calcHist([gamma_corrected], [0], None, [256], [0, 256])
    hist_equalized = cv2.calcHist([equalized], [0], None, [256], [0, 256])

    # 创建可视化窗口
    plt.figure(figsize=(15, 10))
    plt.suptitle('任务二：空间域增强与去噪', fontsize=16, fontweight='bold')

    # 原始灰度图像
    plt.subplot(3, 4, 1)
    plt.imshow(gray, cmap='gray')
    plt.title('原始灰度图像')
    plt.axis('off')

    # 原始灰度直方图
    plt.subplot(3, 4, 2)
    plt.plot(hist_original, 'k', linewidth=1)
    plt.fill_between(range(256), hist_original.flatten(), alpha=0.3, color='gray')
    plt.title('原始直方图')
    plt.xlabel('像素值')
    plt.ylabel('频数')
    plt.grid(True, alpha=0.3)
    plt.xlim([0, 255])

    # Gamma校正结果
    plt.subplot(3, 4, 3)
    plt.imshow(gamma_corrected, cmap='gray')
    plt.title(f'Gamma校正 (γ={gamma})')
    plt.axis('off')

    # Gamma校正直方图
    plt.subplot(3, 4, 4)
    plt.plot(hist_gamma, 'k', linewidth=1)
    plt.fill_between(range(256), hist_gamma.flatten(), alpha=0.3, color='orange')
    plt.title('Gamma校正后直方图')
    plt.xlabel('像素值')
    plt.ylabel('频数')
    plt.grid(True, alpha=0.3)
    plt.xlim([0, 255])

    # 直方图均衡化结果
    plt.subplot(3, 4, 5)
    plt.imshow(equalized, cmap='gray')
    plt.title('直方图均衡化')
    plt.axis('off')

    # 均衡化直方图
    plt.subplot(3, 4, 6)
    plt.plot(hist_equalized, 'k', linewidth=1)
    plt.fill_between(range(256), hist_equalized.flatten(), alpha=0.3, color='blue')
    plt.title('均衡化后直方图')
    plt.xlabel('像素值')
    plt.ylabel('频数')
    plt.grid(True, alpha=0.3)
    plt.xlim([0, 255])

    # 带噪声图像
    plt.subplot(3, 4, 7)
    plt.imshow(noisy, cmap='gray')
    plt.title(f'椒盐噪声 ({noise_density * 100}%)')
    plt.axis('off')

    # 空位（原局部对比位置）
    plt.subplot(3, 4, 8)
    plt.axis('off')

    # 均值滤波结果
    plt.subplot(3, 4, 9)
    plt.imshow(mean_filtered, cmap='gray')
    plt.title('均值滤波 (5×5)')
    plt.axis('off')

    # 中值滤波结果
    plt.subplot(3, 4, 10)
    plt.imshow(median_filtered, cmap='gray')
    plt.title('中值滤波 (5×5)')
    plt.axis('off')

    # 空位
    plt.subplot(3, 4, 11)
    plt.axis('off')

    # 空位
    plt.subplot(3, 4, 12)
    plt.axis('off')

    plt.tight_layout()
    plt.show()

    return gamma_corrected, equalized, median_filtered
# ==================== 任务三：频域分析与周期噪声去除 ====================
    # 解决周期性噪声（如划痕或扫描纹理）问题

def task3_frequency_domain(enhanced_gray):
    """任务三：频域分析与周期噪声去除（修复版）"""
    # 1. 添加周期性噪声（保持原有噪声逻辑不变），模拟老照片
    rows, cols = enhanced_gray.shape
    noisy_image = enhanced_gray.copy().astype(np.float32)

    # 行方向周期噪声：sin(2πi/20) → 周期20
    for i in range(rows):
        noisy_image[i, :] += 30 * np.sin(2 * np.pi * i / 20)
    # 列方向周期噪声：cos(2πj/25) → 周期25
    for j in range(cols):
        noisy_image[:, j] += 20 * np.cos(2 * np.pi * j / 25)

    noisy_image = np.clip(noisy_image, 0, 255).astype(np.uint8)

    # 2. 傅里叶变换（中心对齐）
    dft = np.fft.fft2(noisy_image.astype(float))
    dft_shift = np.fft.fftshift(dft)  # 频谱中心移到图像中间
    magnitude_spectrum = np.log(np.abs(dft_shift) + 1)  # 对数变换便于观察

    # 3. 设计精准的带阻滤波器（核心修改：对准噪声频域亮点）
    filter_mask = np.ones((rows, cols), dtype=np.float32)
    center_row, center_col = rows // 2, cols // 2

    # 步骤1：计算周期噪声对应的频域亮点坐标
    # 行方向噪声周期20 → 频域偏移量：rows/20
    row_noise_offset = int(rows / 20)
    # 列方向噪声周期25 → 频域偏移量：cols/25
    col_noise_offset = int(cols / 25)

    # 步骤2：定义小半径带阻区域（只覆盖噪声亮点，不影响其他频率）
    noise_radius = 5  # 小半径，精准覆盖噪声亮点即可

    # 抑制行方向噪声对应的频域亮点（中心上下偏移row_noise_offset）
    for i in range(rows):
        for j in range(cols):
            # 亮点1：中心正上方 (center_row - row_noise_offset, center_col)
            dist1 = np.sqrt((i - (center_row - row_noise_offset)) ** 2 + (j - center_col) ** 2)
            # 亮点2：中心正下方 (center_row + row_noise_offset, center_col)
            dist2 = np.sqrt((i - (center_row + row_noise_offset)) ** 2 + (j - center_col) ** 2)
            # 亮点3：中心正左方 (center_row, center_col - col_noise_offset)
            dist3 = np.sqrt((i - center_row) ** 2 + (j - (center_col - col_noise_offset)) ** 2)
            # 亮点4：中心正右方 (center_row, center_col + col_noise_offset)
            dist4 = np.sqrt((i - center_row) ** 2 + (j - (center_col + col_noise_offset)) ** 2)

            # 对噪声亮点区域置0（完全抑制），其他区域保留
            if any([dist < noise_radius for dist in [dist1, dist2, dist3, dist4]]):
                filter_mask[i, j] = 0.0

    # 4. 应用频域滤波
    filtered_dft = dft_shift * filter_mask
    magnitude_filtered = np.log(np.abs(filtered_dft) + 1)

    # 5. 逆傅里叶变换（还原空间域图像）
    filtered_shift = np.fft.ifftshift(filtered_dft)  # 反中心对齐
    restored_image = np.fft.ifft2(filtered_shift)  # 逆DFT
    restored_image = np.abs(restored_image).astype(np.uint8)  # 取幅值并转格式

    # 创建可视化窗口（保持原有布局）
    plt.figure(figsize=(15, 8))
    plt.suptitle('任务三：频域分析与周期噪声去除', fontsize=16, fontweight='bold')

    # 原始增强图像
    plt.subplot(2, 4, 1)
    plt.imshow(enhanced_gray, cmap='gray')
    plt.title('原始增强图像')
    plt.axis('off')

    # 添加周期性噪声后的图像
    plt.subplot(2, 4, 2)
    plt.imshow(noisy_image, cmap='gray')
    plt.title('添加周期性噪声')
    plt.axis('off')

    # 傅里叶频谱图（噪声）
    plt.subplot(2, 4, 3)
    plt.imshow(magnitude_spectrum, cmap='gray')
    plt.title('傅里叶频谱图（含噪声亮点）')
    plt.axis('off')
    plt.colorbar(shrink=0.8)

    # 带阻滤波器模板（精准版）
    plt.subplot(2, 4, 4)
    plt.imshow(filter_mask, cmap='gray')
    plt.title('精准带阻滤波器（小区域）')
    plt.axis('off')
    plt.colorbar(shrink=0.8)

    # 滤波后的频谱图（噪声被抑制）
    plt.subplot(2, 4, 5)
    plt.imshow(magnitude_filtered, cmap='gray')
    plt.title('滤波后频谱图（噪声亮点消失）')
    plt.axis('off')
    plt.colorbar(shrink=0.8)

    # 复原后的图像（清晰无噪声）
    plt.subplot(2, 4, 6)
    plt.imshow(restored_image, cmap='gray')
    plt.title('频域滤波复原图像')
    plt.axis('off')

    # 局部对比：噪声图像 vs 复原图像
    h, w = enhanced_gray.shape
    roi_y, roi_x = h // 2 - 50, w // 2 - 50
    roi_size = 100
    roi_noisy = noisy_image[roi_y:roi_y + roi_size, roi_x:roi_x + roi_size]
    roi_restored = restored_image[roi_y:roi_y + roi_size, roi_x:roi_x + roi_size]

    plt.subplot(2, 4, 7)
    plt.imshow(np.hstack([roi_noisy, roi_restored]), cmap='gray')
    plt.title('局部对比：噪声 vs 复原')
    plt.axis('off')

    # 空位
    plt.subplot(2, 4, 8)
    plt.axis('off')

    plt.tight_layout()
    plt.show()

    return restored_image
# ==================== 任务四：彩色图像处理与增强 ====================
    # 解决彩色图像的对比度、亮度等问题，并进行增强处理

def task4_color_processing(img_rgb):
    """任务四：彩色图像处理与增强"""
    # 1. RGB到HSV颜色空间转换
    img_bgr = cv2.cvtColor(img_rgb, cv2.COLOR_RGB2BGR)
    img_hsv = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2HSV)

    # 分离HSV通道
    h, s, v = cv2.split(img_hsv)

    # 2. 对V（亮度）分量进行直方图均衡化
    v_equalized = cv2.equalizeHist(v)

    # 3. 合并HSV通道并转换回RGB
    hsv_equalized = cv2.merge([h, s, v_equalized])
    rgb_from_hsv = cv2.cvtColor(hsv_equalized, cv2.COLOR_HSV2RGB)

    # 4. 对比方法：直接对RGB图像进行Gamma校正
    gamma = 1.8
    rgb_gamma = np.zeros_like(img_rgb, dtype=np.float32)

    for i in range(3):
        normalized = img_rgb[:, :, i] / 255.0
        gamma_corrected = np.power(normalized, 1 / gamma)
        rgb_gamma[:, :, i] = gamma_corrected * 255

    rgb_gamma = rgb_gamma.astype(np.uint8)

    # 5. 计算直方图
    hist_v_original = cv2.calcHist([v], [0], None, [256], [0, 256])
    hist_v_equalized = cv2.calcHist([v_equalized], [0], None, [256], [0, 256])

    # 创建可视化窗口
    plt.figure(figsize=(16, 8))
    plt.suptitle('任务四：彩色图像处理与增强', fontsize=16, fontweight='bold')

    # 原始RGB图像
    plt.subplot(2, 4, 1)
    plt.imshow(img_rgb)
    plt.title('原始RGB图像')
    plt.axis('off')

    # RGB Gamma校正结果
    plt.subplot(2, 4, 2)
    plt.imshow(rgb_gamma)
    plt.title(f'RGB Gamma校正 (γ={1 / gamma:.2f})')
    plt.axis('off')

    # HSV处理后的RGB图像
    plt.subplot(2, 4, 3)
    plt.imshow(rgb_from_hsv)
    plt.title('HSV处理后RGB图像')
    plt.axis('off')

    # HSV空间V分量（原始）
    plt.subplot(2, 4, 4)
    plt.imshow(v, cmap='gray')
    plt.title('HSV空间V分量（原始亮度）')
    plt.axis('off')

    # 原始V分量直方图
    plt.subplot(2, 4, 5)
    plt.plot(hist_v_original, 'k', linewidth=1)
    plt.fill_between(range(256), hist_v_original.flatten(), alpha=0.3, color='gray')
    plt.title('原始V分量直方图')
    plt.xlabel('像素值')
    plt.ylabel('频数')
    plt.grid(True, alpha=0.3)
    plt.xlim([0, 255])

    # 均衡化后V分量
    plt.subplot(2, 4, 6)
    plt.imshow(v_equalized, cmap='gray')
    plt.title('均衡化后V分量')
    plt.axis('off')

    # 均衡化V分量直方图
    plt.subplot(2, 4, 7)
    plt.plot(hist_v_equalized, 'k', linewidth=1)
    plt.fill_between(range(256), hist_v_equalized.flatten(), alpha=0.3, color='orange')
    plt.title('均衡化V分量直方图')
    plt.xlabel('像素值')
    plt.ylabel('频数')
    plt.grid(True, alpha=0.3)
    plt.xlim([0, 255])

    # 空位
    plt.subplot(2, 4, 8)
    plt.axis('off')

    plt.tight_layout()
    plt.show()

    return rgb_from_hsv


# ==================== 任务五：形态学处理与边缘锐化 ====================
    # 解决边缘模糊、细节丢失等问题，并进行锐化处理

def task5_morphology_sharpening(enhanced_gray):
    """任务五：形态学处理与边缘锐化"""
    # 1. 二值化处理
    binary = cv2.adaptiveThreshold(
        enhanced_gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
        cv2.THRESH_BINARY, 11, 2
    )

    # 2. 形态学操作
    kernel = np.ones((3, 3), np.uint8)
    eroded = cv2.erode(binary, kernel, iterations=1)
    dilated = cv2.dilate(eroded, kernel, iterations=1)
    opened = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel)
    closed = cv2.morphologyEx(binary, cv2.MORPH_CLOSE, kernel)

    # 3. 边缘锐化
    # 拉普拉斯算子
    laplacian = cv2.Laplacian(enhanced_gray, cv2.CV_64F)
    laplacian_abs = np.uint8(np.absolute(laplacian))
    sharpened = cv2.addWeighted(enhanced_gray, 1.5, laplacian_abs, -0.5, 0)
    sharpened = np.clip(sharpened, 0, 255).astype(np.uint8)

    # Sobel算子
    sobel_x = cv2.Sobel(enhanced_gray, cv2.CV_64F, 1, 0, ksize=3)
    sobel_y = cv2.Sobel(enhanced_gray, cv2.CV_64F, 0, 1, ksize=3)
    sobel_magnitude = np.sqrt(sobel_x ** 2 + sobel_y ** 2)
    sobel_magnitude = np.uint8(255 * sobel_magnitude / np.max(sobel_magnitude))
    sobel_sharpened = cv2.addWeighted(enhanced_gray, 1.2, sobel_magnitude, 0.5, 0)
    sobel_sharpened = np.clip(sobel_sharpened, 0, 255).astype(np.uint8)

    # 创建可视化窗口
    plt.figure(figsize=(16, 10))
    plt.suptitle('任务五：形态学处理与边缘锐化', fontsize=16, fontweight='bold')

    # 原始增强图像
    plt.subplot(3, 4, 1)
    plt.imshow(enhanced_gray, cmap='gray')
    plt.title('原始增强图像')
    plt.axis('off')

    # 二值化结果
    plt.subplot(3, 4, 2)
    plt.imshow(binary, cmap='gray')
    plt.title('自适应二值化')
    plt.axis('off')

    # 腐蚀操作结果
    plt.subplot(3, 4, 3)
    plt.imshow(eroded, cmap='gray')
    plt.title('腐蚀操作 (3×3)')
    plt.axis('off')

    # 开运算结果
    plt.subplot(3, 4, 4)
    plt.imshow(opened, cmap='gray')
    plt.title('开运算（先腐蚀后膨胀）')
    plt.axis('off')

    # 拉普拉斯边缘检测
    plt.subplot(3, 4, 5)
    plt.imshow(laplacian_abs, cmap='gray')
    plt.title('拉普拉斯边缘检测')
    plt.axis('off')

    # 拉普拉斯锐化结果
    plt.subplot(3, 4, 6)
    plt.imshow(sharpened, cmap='gray')
    plt.title('拉普拉斯锐化')
    plt.axis('off')

    # Sobel边缘检测
    plt.subplot(3, 4, 7)
    plt.imshow(sobel_magnitude, cmap='gray')
    plt.title('Sobel边缘检测')
    plt.axis('off')

    # Sobel锐化结果
    plt.subplot(3, 4, 8)
    plt.imshow(sobel_sharpened, cmap='gray')
    plt.title('Sobel锐化')
    plt.axis('off')

    # 局部对比：二值化 vs 开运算
    h, w = enhanced_gray.shape
    roi_y, roi_x = h // 2 - 50, w // 2 - 50
    roi_size = 100

    roi_binary = binary[roi_y:roi_y + roi_size, roi_x:roi_x + roi_size]
    roi_opened = opened[roi_y:roi_y + roi_size, roi_x:roi_x + roi_size]

    plt.subplot(3, 4, 9)
    plt.imshow(np.hstack([roi_binary, roi_opened]), cmap='gray')
    plt.title('局部对比：二值化 vs 开运算')
    plt.axis('off')

    # 局部对比：原始 vs 拉普拉斯锐化
    roi_original = enhanced_gray[roi_y:roi_y + roi_size, roi_x:roi_x + roi_size]
    roi_sharpened = sharpened[roi_y:roi_y + roi_size, roi_x:roi_x + roi_size]

    plt.subplot(3, 4, 10)
    plt.imshow(np.hstack([roi_original, roi_sharpened]), cmap='gray')
    plt.title('局部对比：原始 vs 拉普拉斯锐化')
    plt.axis('off')

    # 局部对比：拉普拉斯 vs Sobel边缘检测
    roi_laplacian = laplacian_abs[roi_y:roi_y + roi_size, roi_x:roi_x + roi_size]
    roi_sobel = sobel_magnitude[roi_y:roi_y + roi_size, roi_x:roi_x + roi_size]

    plt.subplot(3, 4, 11)
    plt.imshow(np.hstack([roi_laplacian, roi_sobel]), cmap='gray')
    plt.title('局部对比：拉普拉斯 vs Sobel')
    plt.axis('off')

    # 空位
    plt.subplot(3, 4, 12)
    plt.axis('off')

    plt.tight_layout()
    plt.show()

    return sharpened


# ==================== 主函数：一键运行所有任务 ====================

def main():

    print("\n开始执行老照片修复流程...\n")

    try:
        # 加载老照片
        img_rgb = load_image()
        if img_rgb is None:
            return

        # 任务一：系统初始化与图像质量评估
        print("【任务一】系统初始化与图像质量评估...")
        img_rgb, gray = task1_quality_assessment(img_rgb)

        # 任务二：空间域增强与去噪
        print("【任务二】空间域增强与去噪...")
        gamma_corrected, equalized, median_filtered = task2_spatial_enhancement(gray)

        # 任务三：频域分析与周期噪声去除
        print("【任务三】频域分析与周期噪声去除...")
        # 使用均衡化后的图像作为输入
        freq_filtered = task3_frequency_domain(equalized)

        # 任务四：彩色图像处理与增强
        print("【任务四】彩色图像处理与增强...")
        color_enhanced = task4_color_processing(img_rgb)

        # 任务五：形态学处理与边缘锐化
        print("【任务五】形态学处理与边缘锐化...")
        # 使用中值滤波去噪后的图像作为输入
        sharpened = task5_morphology_sharpening(median_filtered)

        print("\n" + "=" * 70)
        print("所有任务执行完成！")
        print("=" * 70)

    except Exception as e:
        print(f"程序执行过程中出现错误：{e}")
        print("请检查：")
        print("1. 图片路径是否正确")
        print("2. 依赖库是否安装：pip install opencv-python numpy matplotlib")


# ==================== 程序入口 ====================
if __name__ == "__main__":
    main()