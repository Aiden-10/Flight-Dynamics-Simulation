import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os

plt.style.use('dark_background')

# 2. Load and Prepare Data
script_dir = os.path.dirname(__file__) if '__file__' in locals() else os.getcwd()
data_path = os.path.join(script_dir, '..', 'data', 'simulation_log.csv')

try:
    df = pd.read_csv(data_path)
except FileNotFoundError:
    print(f"Error: Could not find {data_path}")
    exit()

df['vel'] = np.sqrt(df['vx']**2 + df['vy']**2 + df['vz']**2)

fig = plt.figure(figsize=(16, 9))
fig.canvas.manager.set_window_title("🚀 Missile Flight Dashboard")
# Create a grid: 3 rows, 4 columns. 3D plot takes first 3 cols, 2D plots take the last col.
gs = fig.add_gridspec(3, 4, width_ratios=[1, 1, 1, 1.2], wspace=0.3, hspace=0.4)

ax3d = fig.add_subplot(gs[:, :3], projection='3d')
ax_vel = fig.add_subplot(gs[0, 3])
ax_aoa = fig.add_subplot(gs[1, 3])
ax_mass = fig.add_subplot(gs[2, 3])

max_val = max(df['x'].max(), df['z'].max(), 10) 
y_lim = max(abs(df['y'].min()), abs(df['y'].max()), 50)

ax3d.set_xlim(0, max_val)
ax3d.set_ylim(-y_lim, y_lim)
ax3d.set_zlim(0, max_val)
ax3d.set_xlabel('Downrange Distance X (m)')
ax3d.set_ylabel('Crossrange Y (m)')
ax3d.set_zlabel('Altitude Z (m)')
ax3d.set_title("3D Spatial Trajectory", fontsize=14, weight='bold', color='cyan')

# --- NEW: Force True Proportions ---
x_span = max_val - 0
y_span = y_lim - (-y_lim)
z_span = max_val - 0
# Pro tip: If altitude is visually lost because downrange is huge, change z_span to `z_span * 2` or `z_span * 3` here to exaggerate it.
ax3d.set_box_aspect((x_span, y_span, z_span))

# --- NEW: Better Default Camera Angle ---
ax3d.view_init(elev=25, azim=-45)

# --- NEW: Style the "Ground" Plane ---
ax3d.xaxis.pane.set_facecolor((0.0, 0.0, 0.0, 0.0))
ax3d.yaxis.pane.set_facecolor((0.0, 0.0, 0.0, 0.0))
ax3d.zaxis.pane.set_facecolor((0.1, 0.1, 0.1, 0.5)) # Faint gray floor
ax3d.xaxis._axinfo["grid"].update({"linewidth": 0})
ax3d.yaxis._axinfo["grid"].update({"linewidth": 0})
ax3d.zaxis._axinfo["grid"].update({"color": "gray", "alpha": 0.5, "linestyle": ":"})

line3d, = ax3d.plot([], [], [], lw=2, color='cyan', label='Flight Path')
point3d, = ax3d.plot([], [], [], 'ro', markersize=8, label='Missile')
shadow3d, = ax3d.plot([], [], [], lw=1, color='white', alpha=0.3)

# --- NEW: The Drop Line ---
drop_line, = ax3d.plot([], [], [], lw=1.5, color='yellow', linestyle='--', alpha=0.7)

# Live Telemetry Text inside 3D plot
telemetry_text = ax3d.text2D(0.05, 0.95, "", transform=ax3d.transAxes, 
                             fontsize=12, family='monospace',
                             bbox=dict(facecolor='black', alpha=0.7, edgecolor='cyan'))

def setup_2d_ax(ax, y_data, title, color):
    # Plot the background line so you can see the whole future path
    ax.plot(df['time'], y_data, color='white', alpha=0.2, lw=1)
    
    line, = ax.plot([], [], color=color, lw=2.5)
    point, = ax.plot([], [], 'o', color=color, markersize=6)
    
    ax.set_xlim(df['time'].min(), df['time'].max())
    y_min, y_max = y_data.min(), y_data.max()
    y_margin = (y_max - y_min) * 0.1 if y_max != y_min else 1
    ax.set_ylim(y_min - y_margin, y_max + y_margin)
    
    ax.set_title(title, color=color, fontsize=12, weight='bold')
    ax.grid(True, linestyle=':', alpha=0.3)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    
    return line, point

line_vel, pt_vel = setup_2d_ax(ax_vel, df['vel'], 'Total Velocity (m/s)', '#00ffcc')
line_aoa, pt_aoa = setup_2d_ax(ax_aoa, df['aoa'], 'Angle of Attack (°)', '#ff00ff')
line_mass, pt_mass = setup_2d_ax(ax_mass, df['mass'], 'Mass (kg)', '#ffcc00')
ax_mass.set_xlabel('Time (s)')

def update(frame):
    t = df['time'][:frame+1]
    x = df['x'][:frame+1]
    y = df['y'][:frame+1]
    z = df['z'][:frame+1]
    
    # Update 3D
    line3d.set_data(x, y)
    line3d.set_3d_properties(z)
    
    curr_x, curr_y, curr_z = x.iloc[-1], y.iloc[-1], z.iloc[-1]
    point3d.set_data([curr_x], [curr_y])
    point3d.set_3d_properties([curr_z])
    
    shadow3d.set_data(x, y)
    shadow3d.set_3d_properties([0] * len(x)) # Flatten Z to 0

    # --- NEW: Update Drop Line ---
    drop_line.set_data([curr_x, curr_x], [curr_y, curr_y])
    drop_line.set_3d_properties([0, curr_z])
    
    # Update 2D Charts
    line_vel.set_data(t, df['vel'][:frame+1])
    pt_vel.set_data([t.iloc[-1]], [df['vel'].iloc[frame]])
    
    line_aoa.set_data(t, df['aoa'][:frame+1])
    pt_aoa.set_data([t.iloc[-1]], [df['aoa'].iloc[frame]])
    
    line_mass.set_data(t, df['mass'][:frame+1])
    pt_mass.set_data([t.iloc[-1]], [df['mass'].iloc[frame]])
    
    # Update Text
    telemetry_text.set_text(f"T+    {t.iloc[-1]:>6.2f} s\n"
                            f"ALT:  {curr_z:>6.1f} m\n"
                            f"VEL:  {df['vel'].iloc[frame]:>6.1f} m/s\n"
                            f"MASS: {df['mass'].iloc[frame]:>6.1f} kg")
    
    current_azim = ax3d.azim  # Capture the current left/right angle
    ax3d.view_init(elev=25, azim=current_azim)
    
    # --- NEW: Return drop_line in the list ---
    return line3d, point3d, shadow3d, drop_line, line_vel, pt_vel, line_aoa, pt_aoa, line_mass, pt_mass, telemetry_text

plt.tight_layout()

# Run the animation
# interval=50 means 20 FPS (50ms per frame). Adjust to speed up or slow down.
ani = animation.FuncAnimation(fig, update, frames=len(df), interval=33, blit=False)

plt.show()