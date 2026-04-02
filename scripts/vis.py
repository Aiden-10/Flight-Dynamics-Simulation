import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os

path = os.path.join(os.getcwd(), '..', 'data', 'simulation_log.csv')
df = pd.read_csv(path)
df['vel'] = np.sqrt(df.vx**2 + df.vy**2 + df.vz**2)

fig = plt.figure(figsize=(16, 9))
bg_color = '#11151C'
panel_color = '#1A1E26'
fig.patch.set_facecolor(bg_color)

gs = fig.add_gridspec(3, 4, width_ratios=[1, 1, 1, 1.2])
ax3d = fig.add_subplot(gs[:, :3], projection='3d')
ax_vel = fig.add_subplot(gs[0, 3])
ax_aoa = fig.add_subplot(gs[1, 3])
ax_mass = fig.add_subplot(gs[2, 3])

max_val = max(df.x.max(), df.z.max(), 10)
y_lim = max(abs(df.y.min()), abs(df.y.max()), 50)

ax3d.set_xlim(0, max_val)
ax3d.set_ylim(-y_lim, y_lim)
ax3d.set_zlim(0, max_val)
ax3d.set_box_aspect((1, 1, 2.5))
ax3d.view_init(elev=18, azim=-70)
ax3d.set_xticks([]); ax3d.set_yticks([]); ax3d.set_zticks([])
ax3d.set_facecolor(bg_color)
ax3d.xaxis.set_pane_color((1,1,1,0)); ax3d.yaxis.set_pane_color((1,1,1,0)); ax3d.zaxis.set_pane_color((1,1,1,0))
ax3d.xaxis.line.set_color((1,1,1,0)); ax3d.yaxis.line.set_color((1,1,1,0)); ax3d.zaxis.line.set_color((1,1,1,0))

xx, yy = np.meshgrid(np.linspace(0, max_val, 20), np.linspace(-y_lim, y_lim, 20))
zz = np.zeros_like(xx)
ax3d.plot_surface(xx, yy, zz, color='gray', alpha=0.12)

# faint grid walls
xz_x, xz_z = np.meshgrid(np.linspace(0,max_val,20), np.linspace(0,max_val,20))
xz_y = np.full_like(xz_x, -y_lim)
ax3d.plot_surface(xz_x, xz_y, xz_z, color='white', alpha=0.03, linewidth=0, shade=False)

yz_y, yz_z = np.meshgrid(np.linspace(-y_lim,y_lim,20), np.linspace(0,max_val,20))
yz_x = np.zeros_like(yz_y)
ax3d.plot_surface(yz_x, yz_y, yz_z, color='white', alpha=0.03, linewidth=0, shade=False)

# axis labels
ax3d.text(max_val, 0, 0, "X", color='white', alpha=0.6)
ax3d.text(0, y_lim, 0, "Y", color='white', alpha=0.6)
ax3d.text(0, 0, max_val, "ALT", color='white', alpha=0.7)

trail_glow, = ax3d.plot([], [], [], lw=8, color='cyan', alpha=0.08)
trail, = ax3d.plot([], [], [], lw=2, color='cyan')
rocket, = ax3d.plot([], [], [], marker='^', color='orange', markersize=12)
shadow, = ax3d.plot([], [], [], color='white', alpha=0.25)
drop, = ax3d.plot([], [], [], '--', color='white', alpha=0.5)

text = ax3d.text2D(0.03, 0.92, "", transform=ax3d.transAxes, fontsize=11, color='white',
                    family='monospace', bbox=dict(facecolor='black', alpha=0.6, edgecolor='cyan'))

def small_plot(ax, y, title, color):
    ax.plot(df.time, y, color='white', alpha=0.15)
    line, = ax.plot([], [], color=color, lw=2)
    pt, = ax.plot([], [], 'o', color=color)
    ax.set_xlim(df.time.min(), df.time.max())
    pad = (y.max() - y.min())*0.1 or 1
    ax.set_ylim(y.min()-pad, y.max()+pad)
    ax.set_title(title, color=color)
    ax.grid(True, color='white', alpha=0.1)
    ax.set_facecolor(panel_color)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['bottom'].set_color('#444C5C')
    ax.spines['left'].set_color('#444C5C')
    ax.tick_params(colors='white')
    return line, pt

line_v, pt_v = small_plot(ax_vel, df.vel, "Velocity", '#00ffcc')
line_a, pt_a = small_plot(ax_aoa, df.aoa, "AOA", '#ff00ff')
line_m, pt_m = small_plot(ax_mass, df.mass, "Mass", '#ffcc00')

def update(i):
    x, y, z = df.x[:i+1], df.y[:i+1], df.z[:i+1]
    t = df.time[:i+1]
    cx, cy, cz = x.iloc[-1], y.iloc[-1], z.iloc[-1]

    trail.set_data(x, y); trail.set_3d_properties(z)
    trail_glow.set_data(x, y); trail_glow.set_3d_properties(z)
    rocket.set_data([cx], [cy]); rocket.set_3d_properties([cz])
    shadow.set_data(x, y); shadow.set_3d_properties([0]*len(x))
    drop.set_data([cx, cx], [cy, cy]); drop.set_3d_properties([0, cz])

    line_v.set_data(t, df.vel[:i+1]); pt_v.set_data([t.iloc[-1]], [df.vel.iloc[i]])
    line_a.set_data(t, df.aoa[:i+1]); pt_a.set_data([t.iloc[-1]], [df.aoa.iloc[i]])
    line_m.set_data(t, df.mass[:i+1]); pt_m.set_data([t.iloc[-1]], [df.mass.iloc[i]])

    text.set_text(f"T+ {t.iloc[-1]:.2f}s\nALT {cz:.0f} m\nVEL {df.vel.iloc[i]:.0f} m/s\nMASS {df.mass.iloc[i]:.0f} kg")
    ax3d.view_init(elev=18, azim=ax3d.azim)

    return trail, trail_glow, rocket, shadow, drop

ani = animation.FuncAnimation(fig, update, frames=len(df), interval=33)
plt.tight_layout()
plt.show()