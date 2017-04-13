# SFML_Particles
A simple SFML project demonstrated a flexible, extensible particle system

A Particle System is a collection of Particles. Each Particle has a position, velocity and lifespan. The system takes care of updating each particle with respect to time, and also stores a 64 x 64 texture and color filter that can be applied to each particle within a system. Additionally, there are controls to set the emission angle, emission position, maximum and minimum starting velocities and gravity for each Particle System. Simple repulsor and attractor points can also be set within each Particle System.

Particle Systems render themselves efficiently, and only one draw call is invoked per rendered frame - no matter how many Particles a system contains.
