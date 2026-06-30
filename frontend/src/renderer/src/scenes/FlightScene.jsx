import { useEffect, useRef } from "react";
import * as THREE from "https://cdn.jsdelivr.net/npm/three@0.160.0/build/three.module.js";

export default function FlightScene({ state }) {
  const mountRef = useRef(null);
  const stateRef = useRef(state);

  useEffect(() => {
    stateRef.current = state;
  }, [state]);

  useEffect(() => {
    const mount = mountRef.current;
    if (!mount) return;

    // --------------------
    // Scene Setup
    // --------------------
    const scene = new THREE.Scene();
    scene.background = new THREE.Color(0x0a0a0a);

    const camera = new THREE.PerspectiveCamera(
      65,
      mount.clientWidth / mount.clientHeight,
      0.1,
      5000
    );

    camera.position.set(0, 8, 18);

    const renderer = new THREE.WebGLRenderer({
      antialias: true,
      alpha: false,
    });

    renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
    renderer.setSize(mount.clientWidth, mount.clientHeight);
    renderer.outputColorSpace = THREE.SRGBColorSpace;
    mount.appendChild(renderer.domElement);

    // --------------------
    // Lighting
    // --------------------
    const ambient = new THREE.AmbientLight(0xffffff, 0.28);
    scene.add(ambient);

    const keyLight = new THREE.DirectionalLight(0xffffff, 1.1);
    keyLight.position.set(8, 12, 10);
    scene.add(keyLight);

    const rimLight = new THREE.DirectionalLight(0x6cff8f, 0.5);
    rimLight.position.set(-8, 4, -10);
    scene.add(rimLight);

    // --------------------
    // Rocket Model
    // --------------------
    const rocket = new THREE.Group();

    const body = new THREE.Mesh(
      new THREE.CylinderGeometry(0.35, 0.35, 3.2, 32),
      new THREE.MeshStandardMaterial({
        color: 0x2a2a2a,
        metalness: 0.6,
        roughness: 0.35,
      })
    );
    body.rotation.x = Math.PI / 2;
    rocket.add(body);

    const nose = new THREE.Mesh(
      new THREE.ConeGeometry(0.35, 0.9, 32),
      new THREE.MeshStandardMaterial({
        color: 0x3a3a3a,
        metalness: 0.6,
        roughness: 0.3,
      })
    );
    nose.rotation.x = Math.PI / 2;
    nose.position.z = 2.05;
    rocket.add(nose);

    const engine = new THREE.Mesh(
      new THREE.CylinderGeometry(0.28, 0.42, 0.45, 32),
      new THREE.MeshStandardMaterial({
        color: 0x111111,
        metalness: 0.8,
        roughness: 0.25,
      })
    );
    engine.rotation.x = Math.PI / 2;
    engine.position.z = -1.85;
    rocket.add(engine);

    // Fins
    const finMaterial = new THREE.MeshStandardMaterial({
      color: 0x1f1f1f,
      metalness: 0.5,
      roughness: 0.4,
    });

    for (let i = 0; i < 4; i++) {
      const fin = new THREE.Mesh(
        new THREE.BoxGeometry(0.08, 0.65, 0.55),
        finMaterial
      );

      const angle = (i / 4) * Math.PI * 2;
      fin.position.set(
        Math.cos(angle) * 0.42,
        Math.sin(angle) * 0.42,
        -1.25
      );

      fin.rotation.z = angle;
      rocket.add(fin);
    }

    // Green centerline indicator
    const centerLine = new THREE.Mesh(
      new THREE.BoxGeometry(0.03, 0.03, 3.9),
      new THREE.MeshBasicMaterial({ color: 0x6cff8f })
    );
    centerLine.position.y = 0.37;
    rocket.add(centerLine);

    scene.add(rocket);

    // --------------------
    // Ground Grid
    // --------------------
    const grid = new THREE.GridHelper(500, 100, 0x3a3a3a, 0x1c1c1c);
    grid.position.y = -2;
    scene.add(grid);

    // --------------------
    // Reusable Camera Vectors
    // --------------------
    const cameraTarget = new THREE.Vector3();
    const cameraOffset = new THREE.Vector3();
    const lookTarget = new THREE.Vector3();

    // --------------------
    // Resize Handling
    // --------------------
    const handleResize = () => {
      const width = mount.clientWidth;
      const height = mount.clientHeight;

      if (width === 0 || height === 0) return;

      renderer.setSize(width, height);
      camera.aspect = width / height;
      camera.updateProjectionMatrix();
    };

    const resizeObserver = new ResizeObserver(handleResize);
    resizeObserver.observe(mount);

    // --------------------
    // Animation Loop
    // --------------------
    let animationId;

    const animate = () => {
      animationId = requestAnimationFrame(animate);

      const currentState = stateRef.current;

      if (currentState) {
        const p = currentState.position;
        const q = currentState.quaternion;

        if (Array.isArray(p) && p.length >= 3) {
          rocket.position.set(p[0], p[1], p[2]);
        }

        if (Array.isArray(q) && q.length >= 4) {
          rocket.quaternion.set(q[0], q[1], q[2], q[3]);
        }
      } else {
        rocket.rotation.y += 0.003;
      }

      // Camera follows from behind and above.
      cameraOffset.set(0, 6, -16);
      cameraOffset.applyQuaternion(rocket.quaternion);

      cameraTarget.copy(rocket.position).add(cameraOffset);
      camera.position.lerp(cameraTarget, 0.06);

      lookTarget.copy(rocket.position);
      camera.lookAt(lookTarget);

      renderer.render(scene, camera);
    };

    animate();

    // --------------------
    // Cleanup
    // --------------------
    return () => {
      cancelAnimationFrame(animationId);
      resizeObserver.disconnect();

      scene.traverse((object) => {
        if (!object.isMesh) return;

        object.geometry?.dispose();

        if (Array.isArray(object.material)) {
          object.material.forEach((material) => material.dispose());
        } else {
          object.material?.dispose();
        }
      });

      renderer.dispose();

      if (renderer.domElement.parentNode === mount) {
        mount.removeChild(renderer.domElement);
      }
    };
  }, []);

  const altitude = state?.position?.[1]?.toFixed?.(1) ?? "----";
  const x = state?.position?.[0]?.toFixed?.(1) ?? "----";
  const y = state?.position?.[1]?.toFixed?.(1) ?? "----";
  const z = state?.position?.[2]?.toFixed?.(1) ?? "----";

  return (
    <div ref={mountRef} className="viewport">
      <div className="hud">
        <div className="hudTopLeft">
          <div>ALT {altitude} M</div>
          <div>POS X {x}</div>
          <div>POS Y {y}</div>
          <div>POS Z {z}</div>
        </div>

        <div className="hudReticle">
          <div className="reticleH" />
          <div className="reticleV" />
        </div>

        <div className="hudBottom">
          <span>GUIDANCE: MANUAL</span>
          <span>FRAME: BODY</span>
          <span>STATUS: NOMINAL</span>
        </div>
      </div>
    </div>
  );
}