import React, { useState } from 'react';
import './Sidebar.css';
import { SidebarData } from './SidebarData';
import { FaBars } from 'react-icons/fa';

const Sidebar = () => {
    const [isExpanded, setIsExpanded] = useState(false);

    return (
        <div>
            {/* Menu Button */}
            <button
                className="menu-button"
                onClick={() => setIsExpanded(!isExpanded)}
            >
                <FaBars />
            </button>

            {/* Sidebar */}
            <div className={`sidebar ${isExpanded ? 'expanded' : 'compact'}`}>
                <ul className="sidebar-list">
                    {SidebarData.map((item, index) => (
                        <li key={index} className="sidebar-row">
                            <div className="icon">{item.icon}</div>
                            {isExpanded && <div className="title">{item.title}</div>}
                        </li>
                    ))}
                </ul>
            </div>
        </div>
    );
};

export default Sidebar;
